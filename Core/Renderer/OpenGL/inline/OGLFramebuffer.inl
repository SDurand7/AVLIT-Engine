namespace AVLIT {

inline void OGLFramebuffer::operator=(OGLFramebuffer &&framebuffer) noexcept {
    m_fboID = framebuffer.m_fboID;
    framebuffer.m_fboID = 0;
    m_rboID = framebuffer.m_rboID;
    framebuffer.m_rboID = 0;
    m_firstToResize = framebuffer.m_firstToResize;
    m_textures = std::move(framebuffer.m_textures);
    m_samplers = std::move(framebuffer.m_samplers);
    m_buffersInfos = std::move(framebuffer.m_buffersInfos);
    m_size = framebuffer.size();
}

inline void OGLFramebuffer::bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_fboID); }

inline void OGLFramebuffer::bindBuffer(int i, int sampler, int texture) const {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, m_textures[texture]);
    glBindSampler(i, m_samplers[sampler]);
}

inline void OGLFramebuffer::setParameters(const std::string&name, OGLShader *shader, GLint unitID) const { shader->setUniform(name, unitID); }

inline void OGLFramebuffer::saveDefault() { glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_defaultID); }

inline void OGLFramebuffer::bindDefault() { glBindFramebuffer(GL_FRAMEBUFFER, m_defaultID); }

inline void OGLFramebuffer::blitDepthBuffer(int width, int height) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_defaultID);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

inline std::pair<GLuint, GLuint> OGLFramebuffer::size() const { return m_size; }

} // namespace AVLIT