#pragma once

#include "glad.h"

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>


namespace AVLIT {

enum class TextureInternalFormat {
    R = GL_RED,
    R16F = GL_R16F,
    R32F = GL_R32F,
    RG = GL_RG,
    RG16F = GL_RG16F,
    RG32F = GL_RG32F,
    RGB = GL_RGB,
    RGB16F = GL_RGB16F,
    RGB32F = GL_RGB32F,
    SRGB = GL_SRGB,
    RGBA = GL_RGBA,
    RGBA16F = GL_RGBA16F,
    RGBA32F = GL_RGBA32F,
    SRGBA = GL_SRGB_ALPHA,
    DEPTH = GL_DEPTH_COMPONENT
};

enum class TextureFormat { R = GL_RED, RG = GL_RG, RGB = GL_RGB, RGBA = GL_RGBA, DEPTH = GL_DEPTH_COMPONENT };

enum class TextureType { TEXTURE2D = GL_TEXTURE_2D, CUBEMAP = GL_TEXTURE_CUBE_MAP };

enum class TextureDataType { UBYTE = GL_UNSIGNED_BYTE, FLOAT = GL_FLOAT };

class OGLTexture {
public:
    OGLTexture() = delete;

    OGLTexture(TextureInternalFormat internalFormat, TextureFormat format, TextureType textureType,
               TextureDataType type = TextureDataType::UBYTE, bool generateMipmap = true);

    ~OGLTexture();

    inline void bind(GLint textureUnit) const;

    template <typename T> void allocate(uint width, uint height, std::remove_reference_t<T &> **data);

    void setParameter(GLenum parameter, GLint value);

    GLuint textureID() const;

    GLuint textureType() const;

private:
    GLuint m_textureID;

    GLint m_internalFormat;

    GLenum m_format;

    GLuint m_textureType;

    GLenum m_dataType;

    bool m_generateMipmap;
};

} // namespace AVLIT

#include <Core/Renderer/OpenGL/inline/OGLTexture.inl>