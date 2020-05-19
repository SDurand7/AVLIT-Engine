#include "MeshManager.hpp"


namespace AVLIT {

Mesh *MeshManager::mesh(const std::string &name) const {
    auto meshIt = m_meshes.find(name);
    if(meshIt == m_meshes.cend())
        return nullptr;
    return meshIt->second.get();
}

Mesh *MeshManager::addMesh(const std::string &name, Material *material, std::vector<uint> &&indices,
                           std::vector<Vec3> &&vertices, std::vector<Vec2> &&texCoords, std::vector<Vec3> &&normals,
                           std::vector<Vec3> &&tangents, std::vector<Vec3> &&bitangents) {
    auto mesh = std::make_unique<Mesh>(material, std::move(indices), std::move(vertices), std::move(texCoords),
                                       std::move(normals), std::move(tangents), std::move(bitangents));
    Mesh *meshptr = mesh.get();
    m_meshes.emplace(name, std::move(mesh));
    return meshptr;
}

} // namespace AVLIT