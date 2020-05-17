namespace ALIT {

inline void OGLShader::bind() const { glUseProgram(m_programID); }

inline void OGLShader::setUniform(int i, int value) { glUniform1i(m_locations[i], value); }

inline void OGLShader::setUniform(int i, uint value) { glUniform1ui(m_locations[i], value); }

inline void OGLShader::setUniform(int i, float value) { glUniform1f(m_locations[i], value); }

inline void OGLShader::setUniform(int i, bool value) { glUniform1i(m_locations[i], value ? 1 : 0); }

inline void OGLShader::setUniform(int i, const Vec3 &value) { glUniform3fv(m_locations[i], 1, value_ptr(value)); }

// inline void OGLShader::setUniform(int i, const Vec4 &value) { glUniform4fv(m_locations[i], 1, value_ptr(value)); }

inline void OGLShader::setUniform(int i, const Mat4 &value) {
    glUniformMatrix4fv(m_locations[i], 1, GL_FALSE, value_ptr(value));
}

} // namespace ALIT