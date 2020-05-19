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

inline void OGLShader::setUniform(GLint location, int value) { glUniform1i(location, value); }

inline void OGLShader::setUniform(GLint location, uint value) { glUniform1ui(location, value); }

inline void OGLShader::setUniform(GLint location, float value) { glUniform1f(location, value); }

inline void OGLShader::setUniform(GLint location, bool value) { glUniform1i(location, value ? 1 : 0); }

inline void OGLShader::setUniform(GLint location, const Vec3 &value) { glUniform3fv(location, 1, value_ptr(value)); }

inline void OGLShader::setUniform(GLint location, const Mat4 &value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}

} // namespace AVLIT