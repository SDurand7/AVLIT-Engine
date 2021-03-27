#include <Core/Base/include/SceneBVHNode.hpp>
#include <Core/Base/include/Model.hpp>


namespace AVLIT {

inline void SceneObject::rotate(const Mat3 &rotation, Vec3 center) {
    _rotate(rotation, center);

    if(m_graphNode)
        m_graphNode->updateParentsAABBs();
}

inline void SceneObject::translate(const Vec3 &translation) {
    _translate(translation);

    if(m_graphNode)
        m_graphNode->updateParentsAABBs();
}

inline const std::string &SceneObject::name() const { return m_name; }

inline const Transform &SceneObject::transform() const { return m_transform; }

inline const Vec3 &SceneObject::position() const { return m_transform[3]; }

inline const Vec3 &SceneObject::front() const { return m_transform[2]; }

inline const Vec3 &SceneObject::right() const { return m_transform[0]; }

inline const Vec3 &SceneObject::up() const { return m_transform[1]; }

inline const AABB &SceneObject::aabb() const { return m_aabb; }

inline bool SceneObject::isVisible() const { return m_visible; }

inline void SceneObject::setTransform(const Transform &trf) { m_transform = trf; }

inline void SceneObject::setTransform(Transform &&trf) { m_transform = std::move(trf); }

inline void SceneObject::_rotate(const Mat3 &transform, const Vec3 &center) {
    Vec3 pos = m_transform[3];

    m_transform[3] = Vec3{0.f};
    m_transform = transform * m_transform;
    m_transform[3] = center + Vec3{transform * Vec4{pos - center, 1.f}};

    if(model()) {
        m_aabb = model()->aabb();
        m_aabb.applyTransform(m_transform);
    } else {
        m_aabb = AABB{pos};
    }

    if(m_graphNode)
        m_graphNode->rotate(transform, center);
}

inline void SceneObject::_translate(const Vec3 &translation) {
    m_transform[3] += translation;
    m_aabb.translate(translation);

    if(m_graphNode)
        m_graphNode->translate(translation);
}

} // namespace AVLIT