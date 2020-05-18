#pragma once

#include <string>
#include <vector>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>


struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace AVLIT {

class AssetManager {
public:
    AssetManager();
    AVLIT_API std::string loadAssetFromFile(const std::string &filepath, const Mat3 &scale = Mat3{1.f}) const;

    // right, left, top, bottom, front, back
    AVLIT_API const Texture *loadSkyboxTexture(const std::vector<std::string> &files) const;

    AVLIT_API const ModelManager *modelManager() const;

private:
    Mesh *processMesh(const aiMesh *mesh, const aiScene *scene, const std::string &directory, const Mat3 &scale) const;

    Material *processMaterial(aiMaterial *material, const std::string &directory) const;

    ModelManagerUptr m_modelManager;
    MeshManagerUptr m_meshManager;
    MaterialManagerUptr m_materialManager;
    TextureManagerUptr m_textureManager;
};

} // namespace AVLIT