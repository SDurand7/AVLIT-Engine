#pragma once

#include <algorithm>
#include <array>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>


namespace AVLIT {

using AABBVertices = std::array<Vec3, 8>;

class AABB {
public:
    AABB() = default;
    explicit AABB(const Vec3 &point);
    explicit AABB(const Vec3 &min, const Vec3 &max);

    void compute(const Model *model, const Transform &transform);

    inline const Vec3 &min() const;
    inline const Vec3 &max() const;

    inline AABBVertices vertices() const;

    inline void merge(const Vec3 &point);
    inline void merge(const AABB &aabb);

    inline void applyTransform(const Transform &transform);

    inline void translate(const Vec3 &translation);

private:
    Vec3 m_min = Vec3{0.f};
    Vec3 m_max = Vec3{0.f};
};

} // namespace AVLIT

#include <Core/Base/inline/AABB.inl>