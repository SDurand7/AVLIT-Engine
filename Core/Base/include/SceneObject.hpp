#ifndef ALIT_SCENEOBJECT_HPP
#define ALIT_SCENEOBJECT_HPP

#include <vector>
#include <string>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Base/include/AABB.hpp>


namespace ALIT {

class SceneObject {
public:
    SceneObject(const std::string &name, const Transform &transform);
    virtual ~SceneObject() = default;

    ALIT_API virtual void scale(const Mat3 &scaling);

    ALIT_API inline void rotate(const Mat3 &rotation, Vec3 center);

    ALIT_API inline void translate(const Vec3 &translation);

    ALIT_API inline const std::string &name();

    ALIT_API inline const Transform &transform() const;

    ALIT_API inline const Vec3 &position() const;

    ALIT_API inline const Vec3 &front() const;

    ALIT_API inline const Vec3 &right() const;

    ALIT_API inline const Vec3 &up() const;

    inline const AABB &aabb() const;

    inline bool isVisible() const;

    inline void setTransform(const Transform &transform);

    inline void setTransform(Transform &&trf);

    ALIT_API SceneBVHNode *graphNode() const;

    ALIT_API void setGraphNode(SceneBVHNode *graphNode);

    ALIT_API void setName(const std::string &name);

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

} // namespace ALIT

#include <Core/Base/inline/SceneObject.inl>

#endif