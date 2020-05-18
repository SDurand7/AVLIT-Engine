#pragma once

#include "glad.h"

#include <string>
#include <vector>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>


namespace AVLIT {

enum class OGLShaderStage { COMPUTE, VERTEX, GEOMETRY, FRAGMENT };

class OGLShader {
public:
    OGLShader() = delete;
    OGLShader(const OGLShader &) = delete;
    void operator=(const OGLShader &) = delete;

    OGLShader(const std::string &vertexFilename, const std::string &fragmentFilename);
    OGLShader(OGLShader &&shader) noexcept;
    ~OGLShader();

    inline void bind() const;

    inline void setUniform(int i, int value);
    inline void setUniform(int i, uint value);
    inline void setUniform(int i, float value);
    inline void setUniform(int i, bool value);
    inline void setUniform(int i, const Vec3 &value);
    // inline void setUniform(int i, const Vec4 &value);
    inline void setUniform(int i, const Mat4 &value);

    static const char *prefix;

private:
    GLuint parseGLSL(GLenum shaderType, const std::string &filename, std::vector<std::string> &uniforms);
    void parseIncludes(std::string &src) const;

    GLuint m_programID;
    std::vector<GLint> m_locations;
};

} // namespace AVLIT

#include <Core/Renderer/OpenGL/inline/OGLShader.inl>