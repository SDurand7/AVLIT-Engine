#ifndef ALIT_CAMERA_HPP
#define ALIT_CAMERA_HPP

#include <Core/Base/include/SceneObject.hpp>
#include <Core/Base/include/Math.hpp>


namespace ALIT {

class Camera : public SceneObject {
public:
    // transform should be a composition of a rotation and a translation, debug models should be scaled on load
    ALIT_API Camera(const std::string &name, const Transform &transform, int width, int height, float fovy, float near,
           float far);

    ALIT_API inline Mat4 view() const;

    ALIT_API inline const Mat4 &projection() const;

    ALIT_API inline const Mat4 &inverseProjection() const;

    ALIT_API inline int width() const;

    ALIT_API inline int height() const;

    ALIT_API inline void setWidth(int width);

    ALIT_API inline void setHeight(int height);

    ALIT_API inline float fovx() const;

    ALIT_API inline float fovy() const;

    ALIT_API inline void setFovy(float angle);

    ALIT_API inline float nearZ() const;
    
    ALIT_API inline float farZ() const;

    ALIT_API inline void setNearZ(float z);
    
    ALIT_API inline void setFarZ(float z);    

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

} // namespace ALIT

#include <Core/Base/inline/Camera.inl>

#endif