namespace AVLIT {

inline void OGLSkybox::bind(const std::string &name, OGLShader *prog, GLuint textureUnit) const {
    m_buffer.bind();
    m_texture->bind(name, prog, textureUnit);
}

inline bool OGLSkybox::hasTexture() const { return m_texture; }

} // namespace AVLIT