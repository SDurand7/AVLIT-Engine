#ifndef ALIT_OGLTEXTURE_HPP
#define ALIT_OGLTEXTURE_HPP

#include "glad.h"

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>


namespace ALIT {

enum class InternalFormat { R = GL_RED, RGB = GL_RGB, SRGB = GL_SRGB, RGBA = GL_RGBA, SRGBA = GL_SRGB_ALPHA };

enum class Format { R = GL_RED, RGB = GL_RGB, RGBA = GL_RGBA };

class OGLTexture {
public:
    OGLTexture() = delete;
    OGLTexture(const uchar *data, int width, int height, InternalFormat internalFormat, Format format);
    OGLTexture(uchar *datas[6], int width[6], int height[6]); // Cube map texture
    ~OGLTexture();

    inline void bind(int i, Shader *shader, int textureUnit, GLenum textureType = GL_TEXTURE_2D) const;

private:
    GLuint m_textureID;
};

} // namespace ALIT

#include <Core/Renderer/OpenGL/inline/OGLTexture.inl>

#endif