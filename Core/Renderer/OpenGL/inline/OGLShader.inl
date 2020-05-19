namespace AVLIT {

inline void OGLShader::bind() const { glUseProgram(m_programID); }

template <typename T> inline void OGLShader::setUniform(const std::string &name, T value) {
    GLint location;

    const auto it = m_uniforms.find(name);
    if(it != m_uniforms.cend()) {
        location = it->second;
        setUniform(location, value);
    } else {
        location = glGetUniformLocation(m_programID, name.c_str());

        if(location != -1) {
            m_uniforms.emplace(name, location);
            setUniform(location, value);
        } else {
            AVLIT_ERROR("Unknown uniform name: " + name);
        }
    }
}

} // namespace AVLIT