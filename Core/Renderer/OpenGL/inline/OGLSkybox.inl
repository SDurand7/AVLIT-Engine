namespace ALIT {

inline void OGLSkybox::bind(int i, OGLShader *prog, GLuint textureUnit) const {
    m_buffer.bind();
    m_texture->bind(i, prog, textureUnit, GL_TEXTURE_CUBE_MAP);
}

inline bool OGLSkybox::hasTexture() const { return m_texture; }

} // namespace ALIT