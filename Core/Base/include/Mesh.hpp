#pragma once

#include <vector>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Base/include/Math.hpp>
#include <Core/Base/include/AABB.hpp>
#include <Core/Base/include/Material.hpp>
#include <Core/Renderer/include/GPUBuffer.hpp>


namespace AVLIT {

class Mesh {
public:
    Mesh(Material *material, std::vector<uint> &&indices, std::vector<Vec3> &&vertices,
         std::vector<Vec2> &&texCoords = {}, std::vector<Vec3> &&normals = {}, std::vector<Vec3> &&colors = {},
         std::vector<Vec3> &&tangents = {}, std::vector<Vec3> &&bitangents = {});

    inline const GPUBuffer &buffer() const;

    inline Material *material() const;

    inline const AABB &aabb() const;

    inline int size() const;

    inline const std::vector<uint> &indices() const;
    inline const std::vector<Vec3> &vertices() const;
    inline const std::vector<Vec2> &texCoords() const;
    inline const std::vector<Vec3> &normals() const;
    inline const std::vector<Vec3> &colors() const;
    inline const std::vector<Vec3> &tangents() const;
    inline const std::vector<Vec3> &bitangents() const;


    inline bool hasTexCoords() const;
    inline bool hasNormals() const;
    inline bool hasColors() const;
    inline bool hasTangentsAndBitangents() const;

private:
    GPUBuffer m_buffer;
    Material *m_material;

    AABB m_localAabb;

    std::vector<uint> m_indices;
    std::vector<Vec3> m_vertices;
    std::vector<Vec2> m_texCoords;
    std::vector<Vec3> m_normals;
    std::vector<Vec3> m_colors;
    std::vector<Vec3> m_tangents;
    std::vector<Vec3> m_bitangents;
};

} // namespace AVLIT

#include <Core/Base/inline/Mesh.inl>