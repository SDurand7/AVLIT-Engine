#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Base/include/Mesh.hpp>


namespace AVLIT {

class MeshManager {
    // TODO: check if two meshes with the same name are actually identical (especially the unamed ones: "")
public:
    MeshManager() = default;

    Mesh *mesh(const std::string &name) const;

    Mesh *addMesh(const std::string &name, Material *material, std::vector<uint> &&indices,
                  std::vector<Vec3> &&vertices, std::vector<Vec2> &&texCoords, std::vector<Vec3> &&normals,
                  std::vector<Vec3> &&tangents, std::vector<Vec3> &&bitangents);

private:
    std::unordered_map<std::string, MeshUptr> m_meshes;
};

} // namespace AVLIT