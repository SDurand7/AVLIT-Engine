#ifndef ALIT_OGLFRAMEBUFFER_HPP
#define ALIT_OGLFRAMEBUFFER_HPP

#include "glad.h"

#include <vector>
#include <tuple>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Renderer/OpenGL/include/OGLShader.hpp>


// This is the messiest thing I've ever done

namespace ALIT {

class OGLFramebuffer {
public:
    OGLFramebuffer() = default;
    OGLFramebuffer(const OGLFramebuffer &) = delete;
    void operator=(const OGLFramebuffer &) = delete;

    // G-Buffer FBO
    OGLFramebuffer(std::vector<std::tuple<GLint, GLenum, GLenum>> &&buffersInfos, GLuint width, GLuint height,
                   GLuint textureUnit);

    // Shadow map FBO
    OGLFramebuffer(GLuint width, GLuint height, GLuint textureUnit); 

    // SSAO FBO
    OGLFramebuffer(GLuint width, GLuint height, GLuint noiseW, GLuint noiseH, GLuint textureUnit);

    // Blur FBO
    OGLFramebuffer(GLuint width, GLuint height, GLuint textureUnit, GLint textureCompareMode); 

    ~OGLFramebuffer();

    inline void operator=(OGLFramebuffer &&framebuffer) noexcept;

    inline void bind() const;

    inline void bindBuffer(int i, int sampler, int texture) const;

    // @brief Binds the textures of the frame buffer to the i-th (and following) uniforms
    inline void setParameters(int i, OGLShader *shader, int unitID) const;

    // Necessary for Qt compatibility
    inline static void saveDefault();

    inline static void bindDefault();

    inline void blitDepthBuffer(int width, int height);

    inline std::pair<GLuint, GLuint> size() const;

    void resize(GLuint width, GLuint height);

private:
    GLuint m_fboID = 0;
    GLuint m_rboID = 0;
    GLuint m_firstToResize = 0;
    static GLint m_defaultID;
    std::vector<GLuint> m_textures;
    std::vector<GLuint> m_samplers;
    std::vector<std::tuple<GLint, GLenum, GLenum>> m_buffersInfos;
    std::pair<GLuint, GLuint> m_size;
};

} // namespace ALIT

#include <Core/Renderer/OpenGL/inline/OGLFramebuffer.inl>

#endif