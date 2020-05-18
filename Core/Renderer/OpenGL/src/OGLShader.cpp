#include "OGLShader.hpp"

#include <fstream>
#include <sstream>
#include <regex>


namespace AVLIT {

const char *OGLShader::prefix = AVLIT_ROOT "/Core/Renderer/OpenGL/GLSL/";

OGLShader::OGLShader(const std::string &vertexFilename, const std::string &fragmentFilename)
    : m_programID{glCreateProgram()} {
    std::vector<std::string> uniforms;
    const GLuint vertexID = parseGLSL(GL_VERTEX_SHADER, vertexFilename, uniforms);
    const GLuint fragID = parseGLSL(GL_FRAGMENT_SHADER, fragmentFilename, uniforms);

    glAttachShader(m_programID, vertexID);
    glAttachShader(m_programID, fragID);
    glLinkProgram(m_programID);

    glDeleteShader(vertexID);
    glDeleteShader(fragID);

    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if(!success) {
        GLint length;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &length);
        std::string log(length, ' ');
        glGetProgramInfoLog(m_programID, length, nullptr, &log[0]);
        LOG("[ERROR]: shader program linking failed with the following:\n" + log);
    }

    m_locations.reserve(uniforms.size());
    for(const auto &name : uniforms) {
        m_locations.push_back(glGetUniformLocation(m_programID, name.c_str()));
    }
}

OGLShader::OGLShader(OGLShader &&shader) noexcept {
    m_programID = shader.m_programID;
    shader.m_programID = 0;
    m_locations = std::move(shader.m_locations);
}

OGLShader::~OGLShader() { glDeleteProgram(m_programID); }

GLuint OGLShader::parseGLSL(GLenum shaderType, const std::string &filename, std::vector<std::string> &uniforms) {
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
            LOG("[ERROR]: shader compilation failed with the following:\n" + log);
        } else {
            const std::regex pattern{"uniform (([A-Z]|[a-z]|[0-9]|_)*) (([A-Z]|[a-z]|[0-9]|_)*);"};
            const std::regex patternArray{
                R"(uniform (([A-Z]|[a-z]|[0-9]|_)*) (([A-Z]|[a-z]|[0-9]|_)*)(\[[1-9][0-9]*\]);)"};
            std::smatch match;

            auto begin = src.cbegin();
            const auto end = src.cend();
            while(regex_search(begin, end, match, pattern)) {
                // TODO: clean with another regex (?)
                if(match[1].str() == "Light") {
                    uniforms.push_back(match[3].str() + ".type");
                    uniforms.push_back(match[3].str() + ".color");
                    uniforms.push_back(match[3].str() + ".position");
                    uniforms.push_back(match[3].str() + ".direction");
                    uniforms.push_back(match[3].str() + ".range");
                    uniforms.push_back(match[3].str() + ".cosInnerAngle");
                    uniforms.push_back(match[3].str() + ".cosOuterAngle");
                    uniforms.push_back(match[3].str() + ".nearZ");
                    uniforms.push_back(match[3].str() + ".farZ");
                } else if(match[1].str() == "Material") {
                    uniforms.push_back(match[3].str() + ".hasNormalTexture");
                    uniforms.push_back(match[3].str() + ".hasKaTexture");
                    uniforms.push_back(match[3].str() + ".hasKdTexture");
                    uniforms.push_back(match[3].str() + ".hasKsTexture");
                    uniforms.push_back(match[3].str() + ".hasAlphaTexture");
                    uniforms.push_back(match[3].str() + ".normalTexture");
                    uniforms.push_back(match[3].str() + ".kaTexture");
                    uniforms.push_back(match[3].str() + ".kdTexture");
                    uniforms.push_back(match[3].str() + ".ksTexture");
                    uniforms.push_back(match[3].str() + ".alphaTexture");
                    uniforms.push_back(match[3].str() + ".ka");
                    uniforms.push_back(match[3].str() + ".kd");
                    uniforms.push_back(match[3].str() + ".ks");
                    uniforms.push_back(match[3].str() + ".ns");
                    uniforms.push_back(match[3].str() + ".nsStrength");
                    uniforms.push_back(match[3].str() + ".alpha");
                } else {
                    uniforms.push_back(match[3].str());
                }
                begin = match.suffix().first;
            }

            // Forces array to be at the end and doesn't support array of structs
            while(regex_search(begin, end, match, patternArray)) {
                int count = std::stoi(match[5].str().substr(1, match[5].str().size() - 2));

                for(int i = 0; i < count; ++i) {
                    uniforms.push_back(match[3].str() + "[" + std::to_string(i) + "]");
                }

                begin = match.suffix().first;
            }
        }
    } else {
        LOG("[ERROR]: could not open file " + filename);
    }

    return id;
}

void OGLShader::parseIncludes(std::string &src) const {
    const std::regex pattern{"#include( )*\"(([A-Z]|[a-z]|[0-9]|_)*\\.glsl)\""};
    std::smatch match;

    auto begin = src.cbegin();
    while(regex_search(begin, src.cend(), match, pattern)) {
        std::ifstream file{prefix + match[2].str()};

        std::stringstream stream;
        stream << file.rdbuf();

        src.replace(match.position(), match.length(), stream.str());
        begin = src.cbegin() + match.position() + stream.str().length();
    }
}

} // namespace AVLIT