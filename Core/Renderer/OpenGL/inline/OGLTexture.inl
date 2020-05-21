#include <Core/Renderer/OpenGL/include/OGLShader.hpp>


namespace AVLIT {

inline void OGLTexture::bind(const std::string &name, OGLShader *shader, GLint textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(m_textureType, m_textureID);
    shader->setUniform(name, textureUnit);
}

} // namespace AVLIT