#pragma once

#include <vector>
#include <string>

#include <Core/Base/include/Types.hpp>
#include <Core/Base/include/AABB.hpp>


namespace AVLIT {

class SceneBVHNode {
public:
    SceneBVHNode() = delete;

    SceneBVHNode(SceneObject *object);

    SceneBVHNode(SceneBVHNode &&node);

    AVLIT_API inline const std::string &name() const;

    AVLIT_API inline int childrenCount() const;

    inline const AABB &aabb() const;

    inline SceneObject *sceneObject() const;

    inline void rotate(const Mat3 &trf, const Vec3 &center);

    inline void scale(const Mat3 &trf);

    inline void translate(const Vec3 &translation);

    void addChild(SceneBVHNode *child);

    AVLIT_API SceneBVHNode *parent() const;

    AVLIT_API void removeChild(SceneBVHNode *child);

    AVLIT_API void transferTo(SceneBVHNode *newParent);

    AVLIT_API SceneBVHNode *child(int i) const;

    void updateAABB();

    void updateParentsAABBs();

private:
    std::vector<SceneBVHNode *> m_children;

    SceneBVHNode *m_parent = nullptr;

    SceneObject *m_sceneObject;

    AABB m_aabb;
};

} // namespace AVLIT

#include <Core/Base/inline/SceneBVHNode.inl>