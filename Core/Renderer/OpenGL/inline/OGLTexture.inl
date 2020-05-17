#include <Core/Renderer/OpenGL/include/OGLShader.hpp>


namespace ALIT {

inline void OGLTexture::bind(int i, Shader *shader, int textureUnit, GLenum textureType) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(textureType, m_textureID);
    shader->setUniform(i, textureUnit);
}

} // namespace ALIT