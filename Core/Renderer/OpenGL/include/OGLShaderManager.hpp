#pragma once

#include "glad.h"

#include <string>
#include <vector>
#include <utility>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Renderer/OpenGL/include/OGLShader.hpp>


namespace AVLIT {

enum class OGLShaderType {
    GBUFFER = 0,
    DEFERRED_LIGHTING = 1,
    SHADOW_MAPPING = 2,
    SKYBOX = 3,
    TONEMAPPING = 4,
    SSAO = 5,
    COUNT = 6
};

using OGLShaderSources = std::vector<std::pair<OGLShaderType, OGLShaderStageFiles>>;

class OGLShaderManager {
public:
    OGLShaderManager() = default;

    OGLShaderManager(const OGLShaderSources &sources);

    inline OGLShader *shader(OGLShaderType type);

private:
    std::vector<OGLShader> m_shaders;
};

} // namespace AVLIT

#include <Core/Renderer/OpenGL/inline/OGLShaderManager.inl>