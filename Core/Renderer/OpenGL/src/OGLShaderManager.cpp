#include "OGLShaderManager.hpp"


namespace AVLIT {

OGLShaderManager::OGLShaderManager(const std::vector<std::pair<std::string, std::string>> &sourceFiles) {
    m_shaders.reserve(sourceFiles.size());
    for(const auto &file : sourceFiles) {
        m_shaders.emplace_back(OGLShader::prefix + file.first, OGLShader::prefix + file.second);
    }
}

} // namespace AVLIT