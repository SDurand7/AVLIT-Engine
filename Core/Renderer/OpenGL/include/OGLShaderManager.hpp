#ifndef ALIT_OGLSHADERMANAGER_HPP
#define ALIT_OGLSHADERMANAGER_HPP

#include "glad.h"

#include <string>
#include <vector>
#include <utility>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Renderer/OpenGL/include/OGLShader.hpp>


namespace ALIT {

enum class OGLShaderType { GBUFFER = 0, DEFERRED_LIGHTING = 1, SHADOW_MAPPING = 2, SKYBOX = 3, TONEMAPPING = 4, SSAO = 5, BLUR = 6, DEBUG = 7 };

class OGLShaderManager {
public:
    OGLShaderManager() = default;
    OGLShaderManager(const std::vector<std::pair<std::string, std::string>> &sourceFiles);

    inline OGLShader *shader(OGLShaderType type);

private:
    std::vector<OGLShader> m_shaders;
};

} // namespace ALIT

#include <Core/Renderer/OpenGL/inline/OGLShaderManager.inl>

#endif