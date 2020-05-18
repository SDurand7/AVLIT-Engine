#include "AABB.hpp"

#include <Core/Base/include/Model.hpp>
#include <Core/Base/include/Mesh.hpp>

#include <iostream>
namespace AVLIT {

AABB::AABB(const Vec3 &point) : m_min{point}, m_max{point} {}

AABB::AABB(const Vec3 &min, const Vec3 &max) : m_min{min}, m_max{max} {}

void AABB::compute(const Model *model, const Transform &transform) {
    const AABB &aabb = model->aabb();
    m_min = aabb.m_min;
    m_max = aabb.m_max;

    applyTransform(transform);
}

} // namespace AVLIT