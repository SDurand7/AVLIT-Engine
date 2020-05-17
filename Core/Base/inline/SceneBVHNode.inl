#include <Core/Base/include/SceneObject.hpp>


namespace ALIT {

inline const std::string &SceneBVHNode::name() const { return m_sceneObject->name(); }

inline int SceneBVHNode::childrenCount() const { return static_cast<int>(m_children.size()); }

inline const AABB &SceneBVHNode::aabb() const { return m_aabb; }

inline SceneObject *SceneBVHNode::sceneObject() const { return m_sceneObject; }

inline void SceneBVHNode::rotate(const Mat3 &rotation, const Vec3 &center) {
    for(const auto &child : m_children) {
        child->m_sceneObject->rotate(rotation, center);
    }
    updateAABB();
}

inline void SceneBVHNode::scale(const Mat3 &scaling) {
    for(const auto &child : m_children) {
        child->m_sceneObject->scale(scaling);
    }
    updateAABB();
}

inline void SceneBVHNode::translate(const Vec3 &translation) {
    for(const auto &child : m_children) {
        child->m_sceneObject->translate(translation);
    }
    updateAABB();
}

} // namespace ALIT