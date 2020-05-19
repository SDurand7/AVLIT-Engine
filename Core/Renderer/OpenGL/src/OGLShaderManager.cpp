#include "OGLShaderManager.hpp"


namespace AVLIT {

OGLShaderManager::OGLShaderManager(const OGLShaderSources &sources)
    : m_shaders(static_cast<int>(OGLShaderType::COUNT)) {
    for(const auto &source : sources) {
        m_shaders[static_cast<int>(source.first)] = Shader(source.second);
    }
}

} // namespace AVLIT