#include <Core/Renderer/OpenGL/include/OGLShader.hpp>


namespace AVLIT {

inline void OGLTexture::bind(const std::string &name, Shader *shader, GLint textureUnit, GLenum textureType) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(textureType, m_textureID);
    shader->setUniform(name, textureUnit);
}

} // namespace AVLIT