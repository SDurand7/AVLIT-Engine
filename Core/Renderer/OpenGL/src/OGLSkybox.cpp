#include "OGLSkybox.hpp"

#include <Core/Base/include/Mesh.hpp>


namespace AVLIT {

OGLSkybox::OGLSkybox(const Texture *texture)
    : m_buffer{Mesh{
          nullptr,
          {0, 1, 3, 3, 1, 2, 1, 5, 2, 2, 5, 6, 5, 4, 6, 6, 4, 7, 4, 0, 7, 7, 0, 3, 3, 2, 7, 7, 2, 6, 4, 5, 0, 0, 5, 1},
          {{-1.f, -1.f, -1.f},
           {1.f, -1.f, -1.f},
           {1.f, 1.f, -1.f},
           {-1.f, 1.f, -1.f},
           {-1.f, -1.f, 1.f},
           {1.f, -1.f, 1.f},
           {1.f, 1.f, 1.f},
           {-1.f, 1.f, 1.f}}}},
      m_texture{texture} {}

void OGLSkybox::operator=(OGLSkybox &&skybox) {
    m_buffer = std::move(skybox.m_buffer);
    m_texture = std::move(skybox.m_texture);
}

} // namespace AVLIT