#include <Core/Renderer/include/Shader.hpp>


namespace AVLIT {

inline Mat4 Light::view() const {
    Mat4 view;

    const Vec3 &translation = m_transform[3];
    const Mat3 &rotation = m_transform;

    view = transpose(rotation);
    view[3][0] = -dot(translation, rotation[0]);
    view[3][1] = -dot(translation, rotation[1]);
    view[3][2] = -dot(translation, rotation[2]);
    view[3][3] = 1.f;

    return view;
}

inline Color3 Light::color() const { return m_color; }

inline void Light::setColor(const Color3 &color) { m_color = color; }

inline bool Light::isLit() const { return m_lit; }

inline void Light::switchState() { m_lit = !m_lit; }

} // namespace AVLIT