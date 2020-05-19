#include "OGLShader.hpp"

#include <fstream>
#include <sstream>
#include <regex>


namespace AVLIT {

OGLShader::OGLShader(const OGLShaderStageFiles &sources) : m_programID{glCreateProgram()} {
    for(const auto &shaderFile : sources) {
        const GLuint id = parseGLSL(static_cast<GLenum>(shaderFile.first),
                                    AVLIT_ROOT "Core/Renderer/OpenGL/GLSL/" + shaderFile.second);

        glAttachShader(m_programID, id);
        glDeleteShader(id);
    }
    glLinkProgram(m_programID);

    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if(!success) {
        GLint length;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &length);
        std::string log(length, ' ');
        glGetProgramInfoLog(m_programID, length, nullptr, &log[0]);

        AVLIT_ERROR("shader program linking failed with the following:\n" + log);
    }
}

OGLShader::OGLShader(OGLShader &&shader) noexcept { moveOperation(std::move(shader)); }

void OGLShader::operator=(OGLShader &&shader) noexcept {
    glDeleteProgram(m_programID);
    moveOperation(std::move(shader));
}

OGLShader::~OGLShader() { glDeleteProgram(m_programID); }

void OGLShader::moveOperation(OGLShader &&shader) noexcept {
    m_programID = shader.m_programID;
    shader.m_programID = 0;
    m_uniforms = std::move(shader.m_uniforms);
}

GLuint OGLShader::parseGLSL(GLenum shaderType, const std::string &filename) {
    std::ifstream file{filename};
    GLuint id = glCreateShader(shaderType);

    if(file.is_open()) {
        std::stringstream stream;
        stream << file.rdbuf();
        std::string src{stream.str()};
        parseIncludes(src);
        const GLchar *const c_src = src.c_str();
        GLint success;
        glShaderSource(id, 1, &c_src, nullptr);
        glCompileShader(id);
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if(!success) {
            GLint length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            std::string log(length, ' ');
            glGetShaderInfoLog(id, length, nullptr, &log[0]);

            AVLIT_ERROR("shader \"" + filename + "\" compilation failed with the following:\n" + log);
        }
    } else {
        AVLIT_ERROR("could not open file " + filename);
    }

    return id;
}

void OGLShader::parseIncludes(std::string &src) const {
    const std::regex pattern{"#include( )*\"(([A-Z]|[a-z]|[0-9]|_)*\\.glsl)\""};
    std::smatch match;

    auto begin = src.cbegin();
    while(regex_search(begin, src.cend(), match, pattern)) {
        std::ifstream file((AVLIT_ROOT "/Core/Renderer/OpenGL/GLSL/") + match[2].str());

        std::stringstream stream;
        stream << file.rdbuf();

        src.replace(match.position(), match.length(), stream.str());
        begin = src.cbegin() + match.position() + stream.str().length();
    }
}

} // namespace AVLIT