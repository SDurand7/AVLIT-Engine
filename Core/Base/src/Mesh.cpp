#include "Mesh.hpp"

#include <Core/Base/include/Material.hpp>


namespace AVLIT {

Mesh::Mesh(Material *material, std::vector<uint> &&indices, std::vector<Vec3> &&vertices, std::vector<Vec2> &&texCoords,
           std::vector<Vec3> &&normals, std::vector<Vec3> &&tangents, std::vector<Vec3> &&bitangents)
    : m_material{material}, m_localAabb{vertices[0]}, m_indices{std::move(indices)}, m_vertices{std::move(vertices)},
      m_texCoords{std::move(texCoords)}, m_normals{std::move(normals)}, m_tangents{std::move(tangents)},
      m_bitangents{std::move(bitangents)} {
    m_buffer = GPUBuffer{*this};

    for(int i = 1; i < static_cast<int>(m_vertices.size()); ++i) {
        m_localAabb.merge(Vec4{m_vertices[i], 1.f});
    }
}

} // namespace AVLIT