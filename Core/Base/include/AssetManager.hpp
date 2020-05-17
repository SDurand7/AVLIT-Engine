#ifndef ALIT_ASSETMANAGER_HPP
#define ALIT_ASSETMANAGER_HPP

#include <string>
#include <vector>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>


struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace ALIT {

class AssetManager {
public:
    AssetManager();
    ALIT_API std::string loadAssetFromFile(const std::string &filepath, const Mat3 &scale = Mat3{1.f}) const;

    // right, left, top, bottom, front, back
    ALIT_API const Texture *loadSkyboxTexture(const std::vector<std::string> &files) const;

    ALIT_API const ModelManager *modelManager() const;

private:
    Mesh *processMesh(const aiMesh *mesh, const aiScene *scene, const std::string &directory, const Mat3 &scale) const;

    Material *processMaterial(aiMaterial *material, const std::string &directory) const;

    ModelManagerUptr m_modelManager;
    MeshManagerUptr m_meshManager;
    MaterialManagerUptr m_materialManager;
    TextureManagerUptr m_textureManager;
};

} // namespace ALIT

#endif