#ifndef ALIT_OGLSKYBOX_HPP
#define ALIT_OGLSKYBOX_HPP

#include "glad.h"

#include <vector>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Renderer/OpenGL/include/OGLVAO.hpp>
#include <Core/Renderer/OpenGL/include/OGLTexture.hpp>


namespace ALIT {

class OGLSkybox {
public:
    OGLSkybox(const OGLSkybox &) = delete;
    void operator=(const OGLSkybox &) = delete;

    OGLSkybox() = default;
    OGLSkybox(const Texture *texture);

    void operator=(OGLSkybox &&skybox);

    inline void bind(int i, OGLShader *prog, GLuint textureUnit) const;

    inline bool hasTexture() const;

private:
    GPUBuffer m_buffer;
    const Texture *m_texture = nullptr;
};

} // namespace ALIT

#include <Core/Renderer/OpenGL/inline/OGLSkybox.inl>

#endif