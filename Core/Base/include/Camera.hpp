#pragma once

#include <Core/Base/include/SceneObject.hpp>
#include <Core/Base/include/Math.hpp>


namespace AVLIT {

class Camera : public SceneObject {
public:
    // transform should be a composition of a rotation and a translation, debug models should be scaled on load
    AVLIT_API Camera(const std::string &name, const Transform &transform, int width, int height, float fovy, float near,
                    float far);

    AVLIT_API inline Mat4 view() const;

    AVLIT_API inline const Mat4 &projection() const;

    AVLIT_API inline const Mat4 &inverseProjection() const;

    AVLIT_API inline int width() const;

    AVLIT_API inline int height() const;

    AVLIT_API inline void setWidth(int width);

    AVLIT_API inline void setHeight(int height);

    AVLIT_API inline float fovx() const;

    AVLIT_API inline float fovy() const;

    AVLIT_API inline void setFovy(float angle);

    AVLIT_API inline float nearZ() const;

    AVLIT_API inline float farZ() const;

    AVLIT_API inline void setNearZ(float z);

    AVLIT_API inline void setFarZ(float z);

private:
    inline void updateProjectionMatrix() const;

    int m_width;
    int m_height;
    float m_fovy;
    float m_near;
    float m_far;

    mutable Mat4 m_projection;
    mutable Mat4 m_invProjection;

    mutable bool m_dirtyProjection;
};

} // namespace AVLIT

#include <Core/Base/inline/Camera.inl>