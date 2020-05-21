#pragma once

#include "glad.h"

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>


namespace AVLIT {

enum class TextureInternalFormat { R = GL_RED, RGB = GL_RGB, SRGB = GL_SRGB, RGBA = GL_RGBA, SRGBA = GL_SRGB_ALPHA };

enum class TextureFormat { R = GL_RED, RGB = GL_RGB, RGBA = GL_RGBA };

enum class TextureType { TEXTURE2D = GL_TEXTURE_2D, CUBEMAP = GL_TEXTURE_CUBE_MAP };

enum class TextureDataType { UCHAR = GL_UNSIGNED_BYTE, FLOAT = GL_FLOAT };

class OGLTexture {
public:
    OGLTexture() = delete;

    OGLTexture(TextureInternalFormat internalFormat, TextureFormat format, TextureType textureType,
               TextureDataType type = TextureDataType::UCHAR, bool generateMipMap = true);

    void allocate(uchar **data, uint width, uint height);

    void setParameter(GLenum parameter, GLint value);

    ~OGLTexture();

    inline void bind(GLint textureUnit) const;

private:
    GLuint m_textureID;

    GLint m_internalFormat;

    GLenum m_format;

    GLuint m_textureType;

    GLenum m_dataType;

    bool m_generateMipMap;
};

} // namespace AVLIT

#include <Core/Renderer/OpenGL/inline/OGLTexture.inl>