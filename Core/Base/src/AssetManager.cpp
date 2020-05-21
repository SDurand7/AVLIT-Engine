#include "AssetManager.hpp"

#include <vector>

#include <Dependencies/assimp/include/assimp/scene.h>
#include <Dependencies/assimp/include/assimp/cimport.h>
#include <Dependencies/assimp/include/assimp/Importer.hpp>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Core/Base/include/stb_image.h>

#include <Core/Base/include/ModelManager.hpp>
#include <Core/Base/include/MeshManager.hpp>
#include <Core/Base/include/TextureManager.hpp>
#include <Core/Base/include/MaterialManager.hpp>


namespace AVLIT {

AssetManager::AssetManager()
    : m_modelManager{std::make_unique<ModelManager>()}, m_meshManager{std::make_unique<MeshManager>()},
      m_materialManager{std::make_unique<MaterialManager>()}, m_textureManager{std::make_unique<TextureManager>()} {}

/// Inspired by https://learnopengl.com/Model-Loading/Model
std::string AssetManager::loadAssetFromFile(const std::string &filepath, const Mat3 &scale) const {
    std::string name;
    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
                                        aiProcess_ImproveCacheLocality | aiProcess_JoinIdenticalVertices |
                                        aiProcess_OptimizeMeshes | aiProcess_GenNormals | aiProcess_GenUVCoords);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        AVLIT_LOG("[ERROR]: Assimp failed to load \"" + filepath +
                  "\" with the following: " + importer.GetErrorString());
        return name;
    }

    auto pos = filepath.find_last_of('/');
    const std::string directory = filepath.substr(0, pos);
    name = filepath.substr(pos + 1, filepath.find_last_of(".") - pos - 1);

    ModelUptr model{std::make_unique<Model>()};
    for(uint i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[i];

        model->addMesh(processMesh(mesh, scene, directory, scale));
    }

    m_modelManager->addModel(name, std::move(model));

    return name;
}

Mesh *AssetManager::processMesh(const aiMesh *mesh, const aiScene *scene, const std::string &directory,
                                const Mat3 &scale) const {
    Mesh *avlitMesh;
    if((avlitMesh = m_meshManager->mesh(mesh->mName.C_Str())))
        return avlitMesh;

    std::vector<uint> indices;
    std::vector<Vec3> vertices;
    std::vector<Vec2> texCoords;
    std::vector<Vec3> normals;
    std::vector<Vec3> colors;
    std::vector<Vec3> tangents;
    std::vector<Vec3> bitangents;

    indices.reserve(mesh->mNumFaces);
    for(uint i = 0; i < mesh->mNumFaces; ++i) {
        indices.push_back(mesh->mFaces[i].mIndices[0]);
        indices.push_back(mesh->mFaces[i].mIndices[1]);
        indices.push_back(mesh->mFaces[i].mIndices[2]);
    }

    vertices.reserve(mesh->mNumVertices);
    for(uint i = 0; i < mesh->mNumVertices; ++i) {
        Vec3 point{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        point = scale * point;
        vertices.emplace_back(point.x, point.y, point.z);
    }

    if(mesh->HasTextureCoords(0)) {
        texCoords.reserve(mesh->mNumVertices);
        for(uint i = 0; i < mesh->mNumVertices; ++i) {
            texCoords.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
    }

    if(mesh->HasNormals()) {
        normals.reserve(mesh->mNumVertices);
        for(uint i = 0; i < mesh->mNumVertices; ++i) {
            Vec3 v{mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
            normals.emplace_back(normalize(v));
        }
    }

    if(mesh->HasTangentsAndBitangents()) {
        tangents.reserve(mesh->mNumVertices);
        bitangents.reserve(mesh->mNumVertices);
        for(uint i = 0; i < mesh->mNumVertices; ++i) {
            Vec3 v{mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
            tangents.emplace_back(normalize(v));
            v = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
            bitangents.emplace_back(normalize(v));
        }
    }

    auto material = processMaterial(scene->mMaterials[mesh->mMaterialIndex], directory);
    return m_meshManager->addMesh(mesh->mName.C_Str(), material, std::move(indices), std::move(vertices),
                                  std::move(texCoords), std::move(normals), std::move(tangents), std::move(bitangents));
}

const Texture *loadTexture(aiMaterial *mat, const std::string &directory, aiTextureType type,
                           TextureManager *textureManager);

Material *AssetManager::processMaterial(aiMaterial *mat, const std::string &directory) const {
    Material *material;
    if((material = m_materialManager->material(mat->GetName().C_Str())) != nullptr)
        return material;

    MaterialUptr newMaterial = std::make_unique<Material>();
    material = newMaterial.get();

    if(mat->GetTextureCount(aiTextureType_HEIGHT) > 0)
        newMaterial->setNormalMap(loadTexture(mat, directory, aiTextureType_HEIGHT, m_textureManager.get()));

    if(mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        newMaterial->setAlbedoMap(loadTexture(mat, directory, aiTextureType_DIFFUSE, m_textureManager.get()));

    if(mat->GetTextureCount(aiTextureType_AMBIENT) > 0)
        newMaterial->setMetalnessMap(loadTexture(mat, directory, aiTextureType_AMBIENT, m_textureManager.get()));

    if(mat->GetTextureCount(aiTextureType_SHININESS) > 0)
        newMaterial->setRoughnessMap(loadTexture(mat, directory, aiTextureType_SHININESS, m_textureManager.get()));

    if(mat->GetTextureCount(aiTextureType_OPACITY) > 0)
        newMaterial->setAlphaMap(loadTexture(mat, directory, aiTextureType_OPACITY, m_textureManager.get()));


    aiColor3D color;
    if(mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
        newMaterial->setAlbedo({color.r, color.g, color.b});

    float alpha;
    if(mat->Get(AI_MATKEY_OPACITY, alpha) == AI_SUCCESS)
        newMaterial->setAlpha(alpha);

    bool twoSided;
    if(mat->Get(AI_MATKEY_TWOSIDED, twoSided) == AI_SUCCESS)
        newMaterial->setTwoSided(twoSided);

    m_materialManager->addMaterial(mat->GetName().C_Str(), std::move(newMaterial));
    return material;
}

const Texture *loadTexture(aiMaterial *mat, const std::string &directory, aiTextureType type,
                           TextureManager *textureManager) {
    aiString path;
    mat->GetTexture(type, 0, &path);
    const std::string fullpath = directory + '/' + path.C_Str();
    const Texture *texture;
    if((texture = textureManager->texture(fullpath)) != nullptr)
        return texture;

    int width, height, intFormat;
    uchar *data = stbi_load(fullpath.c_str(), &width, &height, &intFormat, 0);
    if(!data) {
        AVLIT_LOG("[ERROR]: error loading texture data from " + fullpath);
    } else {
        TextureInternalFormat internalFormat;
        TextureFormat format;

        switch(intFormat) {
        case 1:
            format = TextureFormat::R;
            internalFormat = TextureInternalFormat::R;
            break;
        case 3:
            format = TextureFormat::RGB;

            if(type == aiTextureType_DIFFUSE)
                internalFormat = TextureInternalFormat::SRGB;
            else
                internalFormat = TextureInternalFormat::RGB;
            break;
        case 4:
            format = TextureFormat::RGBA;

            if(type == aiTextureType_DIFFUSE)
                internalFormat = TextureInternalFormat::SRGBA;
            else
                internalFormat = TextureInternalFormat::RGBA;
            break;
        default:
            AVLIT_LOG("[ERROR]: unsupported texture format");
            stbi_image_free(data);
            return nullptr;
        }

        texture =
            textureManager->addTexture(fullpath, &data, width, height, TextureType::TEXTURE2D, internalFormat, format);

        stbi_image_free(data);
    }
    return texture;
}

const Texture *AssetManager::loadSkyboxTexture(const std::vector<std::string> &files) const {
    uchar *data[6];
    int width;
    int height;

    for(int i = 0; i < 6; ++i) {
        int format;

        data[i] = stbi_load(files[i].c_str(), &width, &height, &format, 0);
    }

    std::string name{files[0].substr(0, files[0].find_last_of('/') - 1)};
    AVLIT_LOG(name);
    auto texture = m_textureManager->addTexture(name, data, width, height, TextureType::CUBEMAP);

    for(int i = 0; i < 6; ++i) {
        stbi_image_free(data[i]);
    }

    return texture;
}

const ModelManager *AssetManager::modelManager() const { return m_modelManager.get(); }

} // namespace AVLIT
