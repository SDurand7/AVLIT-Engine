#pragma once

#include "glad.h"

#include <unordered_map>
#include <memory>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Renderer/OpenGL/include/OGLShader.hpp>
#include <Core/Renderer/OpenGL/include/OGLTexture.hpp>


namespace AVLIT {

class OGLFramebuffer {
public:
    OGLFramebuffer(const OGLFramebuffer &) = delete;
    void operator=(const OGLFramebuffer &) = delete;

    explicit OGLFramebuffer(GLuint textureCount);

    ~OGLFramebuffer();

    inline void operator=(OGLFramebuffer &&framebuffer) noexcept;

    inline void bind() const;

    inline OGLTexture *texture(const std::string &name) const;

    inline void copyDepthBuffer(GLuint width, GLuint height, GLuint fboID) const;

    void attachTexture(const std::string &name, std::unique_ptr<OGLTexture> &&texture, GLenum attachment);

    void attachRenderBuffer(GLenum format, GLenum attachment, GLsizei width, GLsizei height);

    void setDrawBuffers(const std::vector<GLenum> &attachments);

    void setDrawBuffer(GLenum buffer);

    void setReadBuffer(GLenum buffer);

    bool isComplete() const;

    void resizeRBO(GLenum format, GLuint width, GLuint height);

    // Mandatory because of Qt
    inline static void saveDefault();

    inline static void bindDefault();

    inline static GLuint defaultID();

private:
    GLuint m_fboID = 0;

    GLuint m_rboID = 0;

    std::unordered_map<std::string, std::unique_ptr<OGLTexture>> m_textures;

    static GLint m_defaultID;
};

} // namespace AVLIT

#include <Core/Renderer/OpenGL/inline/OGLFramebuffer.inl>