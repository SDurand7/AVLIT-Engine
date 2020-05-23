namespace AVLIT {

inline void OGLTexture::bind(GLint textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(m_textureType, m_textureID);

    GL_CHECK_ERROR();
}

template <typename T> void OGLTexture::allocate(uint width, uint height, std::remove_reference_t<T &> **data) {
    if(m_textureType == GL_TEXTURE_2D) {
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, m_format, m_dataType,
                     data ? *data : nullptr);

        if(m_generateMipmap)
            glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        for(int i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_internalFormat, width, height, 0, m_format,
                         m_dataType, data[i]);
        }
    }

    GL_CHECK_ERROR();
}

} // namespace AVLIT