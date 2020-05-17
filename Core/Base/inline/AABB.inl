namespace ALIT {

inline const Vec3 &AABB::min() const { return m_min; }

inline const Vec3 &AABB::max() const { return m_max; }

inline AABBVertices AABB::vertices() const {
    return {m_min,
            m_max,
            {m_min.x, m_max.y, m_min.z},
            {m_max.x, m_max.y, m_min.z},
            {m_max.x, m_min.y, m_min.z},
            {m_max.x, m_min.y, m_max.z},
            {m_min.x, m_min.y, m_max.z},
            {m_min.x, m_max.y, m_max.z}};
}

inline void AABB::merge(const Vec3 &point) {
    m_min = {std::min(m_min.x, point.x), std::min(m_min.y, point.y), std::min(m_min.z, point.z)};
    m_max = {std::max(m_max.x, point.x), std::max(m_max.y, point.y), std::max(m_max.z, point.z)};
}

inline void AABB::merge(const AABB &aabb) {
    m_min = {std::min(m_min.x, aabb.m_min.x), std::min(m_min.y, aabb.m_min.y), std::min(m_min.z, aabb.m_min.z)};
    m_max = {std::max(m_max.x, aabb.m_max.x), std::max(m_max.y, aabb.m_max.y), std::max(m_max.z, aabb.m_max.z)};
}

// cf GraphicsGems 1: "Transforming Axis-Aligned Bounding Boxes" -> this is insane
inline void AABB::applyTransform(const Transform &transform) {
    Vec3 nmin, nmax;
    for(int i = 0; i < 3; ++i) {
        nmin[i] = nmax[i] = transform[3][i];
        for(int j = 0; j < 3; ++j) {
            float a = transform[j][i] * m_min[j];
            float b = transform[j][i] * m_max[j];

            if(a < b) {
                nmin[i] += a;
                nmax[i] += b;
            } else {
                nmin[i] += b;
                nmax[i] += a;
            }
        }
    }
    m_min = nmin;
    m_max = nmax;
}

inline void AABB::translate(const Vec3 &translation) {
    m_min += translation;
    m_max += translation;
}

} // namespace ALIT