#include "OGLTexture.hpp"


namespace AVLIT {


OGLTexture::OGLTexture(TextureInternalFormat internalFormat, TextureFormat format, TextureType textureType,
                       TextureDataType dataType, bool generateMipMap)
    : m_internalFormat{static_cast<GLint>(internalFormat)}, m_format{static_cast<GLenum>(format)},
      m_textureType{static_cast<GLuint>(textureType)}, m_dataType{static_cast<GLenum>(dataType)}, m_generateMipMap{
                                                                                                      generateMipMap} {
    glGenTextures(1, &m_textureID);
    glBindTexture(m_textureType, m_textureID);

    // Default parameters
    if(m_textureType == GL_TEXTURE_2D) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else if(m_textureType == GL_TEXTURE_CUBE_MAP) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    } else
        AVLIT_ERROR("unknown texture type");
}

void OGLTexture::allocate(uchar **data, uint width, uint height) {
    if(m_textureType == GL_TEXTURE_2D) {
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, m_format, m_dataType, *data);

        if(m_generateMipMap)
            glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        for(int i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_internalFormat, width, height, 0, m_format,
                         m_dataType, data[i]);
        }
    }
}

void OGLTexture::setParameter(GLenum parameter, GLint value) { glTexParameteri(m_textureType, parameter, value); }

OGLTexture::~OGLTexture() { glDeleteTextures(1, &m_textureID); }

} // namespace AVLIT