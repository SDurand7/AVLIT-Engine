#include "Camera.hpp"

#include <Core/Base/include/Model.hpp>
#include <Core/Base/include/Mesh.hpp>


namespace AVLIT {

Camera::Camera(const std::string &name, const Transform &transform, int width, int height, float fovy, float near,
               float far)
    : SceneObject{name, transform}, m_width{width}, m_height{height}, m_fovy{fovy}, m_near{near}, m_far{far} {
    m_transform[0] = normalize(m_transform[0]);
    m_transform[1] = normalize(m_transform[1]);
    m_transform[2] = normalize(m_transform[2]);

    updateProjectionMatrix();
}

} // namespace AVLIT