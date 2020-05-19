namespace AVLIT {

inline const GPUBuffer &Mesh::buffer() const { return m_buffer; }

inline Material *Mesh::material() const { return m_material; }

inline const AABB &Mesh::aabb() const { return m_localAabb; }

inline int Mesh::size() const { return static_cast<int>(m_indices.size()); }

inline const std::vector<uint> &Mesh::indices() const { return m_indices; }

inline const std::vector<Vec3> &Mesh::vertices() const { return m_vertices; }

inline const std::vector<Vec2> &Mesh::texCoords() const { return m_texCoords; }

inline const std::vector<Vec3> &Mesh::normals() const { return m_normals; }

inline const std::vector<Vec3> &Mesh::tangents() const { return m_tangents; }

inline const std::vector<Vec3> &Mesh::bitangents() const { return m_bitangents; }

inline bool Mesh::hasTexCoords() const { return !m_texCoords.empty(); }

inline bool Mesh::hasNormals() const { return !m_normals.empty(); }

inline bool Mesh::hasTangentSpace() const { return !m_tangents.empty() && !m_bitangents.empty(); }


} // namespace AVLIT