namespace AVLIT {

inline void OGLFramebuffer::operator=(OGLFramebuffer &&framebuffer) noexcept {
    m_fboID = framebuffer.m_fboID;
    framebuffer.m_fboID = 0;
    m_rboID = framebuffer.m_rboID;
    framebuffer.m_rboID = 0;
    m_textures = std::move(framebuffer.m_textures);
}

inline void OGLFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
    GL_CHECK_ERROR();
}

inline Texture *OGLFramebuffer::texture(const std::string &name) const {
    auto it = m_textures.find(name);

    if(it != m_textures.cend())
        return it->second.get();
    return nullptr;
}

inline void OGLFramebuffer::copyDepthBuffer(GLuint width, GLuint height, GLuint fboID) const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    GL_CHECK_ERROR();
}

inline void OGLFramebuffer::saveDefault() {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_defaultID);
    GL_CHECK_ERROR();
}

inline void OGLFramebuffer::bindDefault() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultID);
    GL_CHECK_ERROR();
}

inline GLuint OGLFramebuffer::defaultID() { return m_defaultID; }

} // namespace AVLIT