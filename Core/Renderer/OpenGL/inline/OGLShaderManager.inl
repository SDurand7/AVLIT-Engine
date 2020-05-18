namespace AVLIT {

inline OGLShader *OGLShaderManager::shader(OGLShaderType type) { return &m_shaders[static_cast<int>(type)]; }

} // namespace AVLIT