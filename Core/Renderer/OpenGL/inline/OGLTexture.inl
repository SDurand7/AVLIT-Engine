#include <Core/Renderer/OpenGL/include/OGLShader.hpp>


namespace AVLIT {

inline void OGLTexture::bind(GLint textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(m_textureType, m_textureID);
}

} // namespace AVLIT