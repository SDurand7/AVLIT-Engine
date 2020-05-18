#pragma once

#include <vector>
#include <string>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Base/include/AABB.hpp>


namespace AVLIT {

class SceneObject {
public:
    SceneObject(const std::string &name, const Transform &transform);
    virtual ~SceneObject() = default;

    AVLIT_API virtual void scale(const Mat3 &scaling);

    AVLIT_API inline void rotate(const Mat3 &rotation, Vec3 center);

    AVLIT_API inline void translate(const Vec3 &translation);

    AVLIT_API inline const std::string &name();

    AVLIT_API inline const Transform &transform() const;

    AVLIT_API inline const Vec3 &position() const;

    AVLIT_API inline const Vec3 &front() const;

    AVLIT_API inline const Vec3 &right() const;

    AVLIT_API inline const Vec3 &up() const;

    inline const AABB &aabb() const;

    inline bool isVisible() const;

    inline void setTransform(const Transform &transform);

    inline void setTransform(Transform &&trf);

    AVLIT_API SceneBVHNode *graphNode() const;

    AVLIT_API void setGraphNode(SceneBVHNode *graphNode);

    AVLIT_API void setName(const std::string &name);

    virtual const Model *model() const;

protected:
    std::string m_name;
    Transform m_transform; // Should probably also store local transform somewhere
    AABB m_aabb;
    SceneBVHNode *m_graphNode = nullptr;

    bool m_visible : 1;

private:
    inline void _rotate(const Mat3 &rotation, const Vec3 &center);

    inline void _translate(const Vec3 &translation);
};

} // namespace AVLIT

#include <Core/Base/inline/SceneObject.inl>