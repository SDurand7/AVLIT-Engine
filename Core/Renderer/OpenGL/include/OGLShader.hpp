#pragma once

#include "glad.h"

#include <string>
#include <vector>
#include <unordered_map>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>


namespace AVLIT {

enum class OGLShaderStage {
    COMPUTE = GL_COMPUTE_SHADER,
    VERTEX = GL_VERTEX_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER
};

using OGLShaderStageFiles = std::vector<std::pair<OGLShaderStage, std::string>>;

class OGLShader {
public:
    OGLShader(const OGLShader &) = delete;
    void operator=(const OGLShader &) = delete;

    OGLShader() = default;

    OGLShader(const OGLShaderStageFiles &sources);

    OGLShader(OGLShader &&shader) noexcept;

    void operator=(OGLShader &&shader) noexcept;

    ~OGLShader();

    inline void bind() const;

    template <typename T> inline void setUniform(const std::string &name, T value);

private:
    GLuint m_programID;

    std::unordered_map<std::string, GLint> m_uniforms;

private:
    void moveOperation(OGLShader &&shader) noexcept;

    GLuint parseGLSL(GLenum shaderType, const std::string &filename);

    void parseIncludes(std::string &src) const;

    inline void setUniform(GLint location, int value);

    inline void setUniform(GLint location, uint value);

    inline void setUniform(GLint location, float value);

    inline void setUniform(GLint location, bool value);

    inline void setUniform(GLint location, const Vec3 &value);

    inline void setUniform(GLint location, const Mat4 &value);
};

} // namespace AVLIT

#include <Core/Renderer/OpenGL/inline/OGLShader.inl>