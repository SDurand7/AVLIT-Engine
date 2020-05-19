namespace AVLIT {

inline void OGLRenderSystem::setCurrentCamera(Camera *camera) {
    if(!m_camera) {
        m_GBuffer = OGLFramebuffer{{{GL_RGBA32F, GL_RGBA, GL_FLOAT},
                                    {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE},
                                    {GL_RG, GL_RGB, GL_UNSIGNED_BYTE}},
                                   static_cast<GLuint>(camera->width()),
                                   static_cast<GLuint>(camera->height()),
                                   5};
        m_ssaoFBO =
            OGLFramebuffer{static_cast<GLuint>(camera->width()), static_cast<GLuint>(camera->height()), 4, 4, 10};
        m_blurFBO = OGLFramebuffer{static_cast<GLuint>(camera->width()), static_cast<GLuint>(camera->height()), 15};
        m_shadowMap = OGLFramebuffer{2048, 2048, 12, GL_COMPARE_REF_TO_TEXTURE};
        m_hdrFBO = OGLFramebuffer{{{GL_RGBA32F, GL_RGBA, GL_FLOAT}},
                                  static_cast<GLuint>(camera->width()),
                                  static_cast<GLuint>(camera->height()),
                                  14};
        setupTextureUnits();
    }
    m_camera = camera;
}

inline void OGLRenderSystem::setSkybox(const Texture *texture) { m_skybox = OGLSkybox{texture}; }

inline void OGLRenderSystem::resize(int width, int height) {
    m_GBuffer.resize(width, height);
    m_ssaoFBO.resize(width, height);
    m_blurFBO.resize(width, height);
    m_hdrFBO.resize(width, height);
}

} // namespace AVLIT