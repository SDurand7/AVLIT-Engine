#include "OGLTexture.hpp"


namespace AVLIT {


OGLTexture::OGLTexture(TextureInternalFormat internalFormat, TextureFormat format, TextureType textureType,
                       TextureDataType dataType, bool generateMipmap)
    : m_internalFormat{static_cast<GLint>(internalFormat)}, m_format{static_cast<GLenum>(format)},
      m_textureType{static_cast<GLuint>(textureType)}, m_dataType{static_cast<GLenum>(dataType)}, m_generateMipmap{
                                                                                                      generateMipmap} {
    // Silence warnings with clang VC19 integration
    std::ignore = m_internalFormat;
    std::ignore = m_format;
    std::ignore = m_dataType;
    
    glGenTextures(1, &m_textureID);
    glBindTexture(m_textureType, m_textureID);

    // Default parameters
    if(m_textureType == GL_TEXTURE_2D) {
        if(m_generateMipmap)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    } else if(m_textureType == GL_TEXTURE_CUBE_MAP) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    } else
        AVLIT_ERROR("unknown texture type");

    GL_CHECK_ERROR();
}

OGLTexture::~OGLTexture() { glDeleteTextures(1, &m_textureID); }

void OGLTexture::setParameter(GLenum parameter, GLint value) { glTexParameteri(m_textureType, parameter, value); }

GLuint OGLTexture::textureID() const { return m_textureID; }

GLuint OGLTexture::textureType() const { return m_textureType; }

} // namespace AVLIT