#include "OGLFramebuffer.hpp"

#include <random>
#include <cmath>

#include <Core/Base/include/Math.hpp>
#include <Core/Renderer/OpenGL/include/OGLTexture.hpp>

#include "bluenoise.h"


namespace AVLIT {
    
GLint OGLFramebuffer::m_defaultID = -1;

OGLFramebuffer::OGLFramebuffer(GLuint textureCount) : m_textures(textureCount) { glGenFramebuffers(1, &m_fboID); }

OGLFramebuffer::~OGLFramebuffer() {
    glDeleteRenderbuffers(1, &m_rboID);
    glDeleteFramebuffers(1, &m_fboID);
}

void OGLFramebuffer::attachTexture(const std::string &name, std::unique_ptr<OGLTexture> &&texture, GLenum attachment) {
    AVLIT_ASSERT(texture->textureType() == GL_TEXTURE_2D, "invalid texture type");
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->textureID(), 0);

    m_textures[name] = std::move(texture);
    GL_CHECK_ERROR();
}

void OGLFramebuffer::attachRenderBuffer(GLenum format, GLenum attachment, GLsizei width, GLsizei height) {
    if(m_rboID == 0) {
        glGenRenderbuffers(1, &m_rboID);
        glBindRenderbuffer(GL_RENDERBUFFER, m_rboID);
        glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_rboID);
    } else
        AVLIT_ERROR("the framebuffer " + std::to_string(m_fboID) + " already have a render buffer");
    GL_CHECK_ERROR();
}

void OGLFramebuffer::setDrawBuffers(const std::vector<GLenum> &attachments) {
    glDrawBuffers(attachments.size(), attachments.data());
    GL_CHECK_ERROR();
}

void OGLFramebuffer::setDrawBuffer(GLenum buffer) {
    glDrawBuffer(buffer);
    GL_CHECK_ERROR();
}

void OGLFramebuffer::setReadBuffer(GLenum buffer) {
    glReadBuffer(buffer);
    GL_CHECK_ERROR();
}

bool OGLFramebuffer::isComplete() const { return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE; }

void OGLFramebuffer::resizeRBO(GLenum format, GLuint width, GLuint height) {
    if(m_rboID != 0) {
        glBindRenderbuffer(GL_RENDERBUFFER, m_rboID);
        glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
        GL_CHECK_ERROR();
    }
}

void OGLFramebuffer::setupShadowMap(uint width, uint height) {
    bind();

    auto texture = std::make_unique<Texture>(TextureInternalFormat::DEPTH, TextureFormat::DEPTH, TextureType::TEXTURE2D,
                                             TextureDataType::FLOAT, false);
    texture->bind(1);
    texture->allocate<nullptr_t>(width, height, nullptr);
    texture->setParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    attachTexture("shadowMap", std::move(texture), GL_DEPTH_ATTACHMENT);
    setDrawBuffer(GL_NONE);
    setReadBuffer(GL_NONE);

    if(!isComplete())
        AVLIT_ERROR("incomplete shadow map FBO");
}

} // namespace AVLIT