#pragma once

#include "glad.h"

#include <vector>
#include <utility>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Base/include/Camera.hpp>
#include <Core/Renderer/OpenGL/include/OGLShaderManager.hpp>
#include <Core/Renderer/OpenGL/include/OGLFramebuffer.hpp>
#include <Core/Renderer/OpenGL/include/OGLVAO.hpp>


namespace AVLIT {

using OGLTextureUptr = std::unique_ptr<OGLTexture>;

class OGLRenderSystem {
public:
    // This needs to be called before anything else
    static void initGL();

    OGLRenderSystem(const std::vector<DrawableUptr> &drawables, const std::vector<LightUptr> &lights, Camera *camera);

    void reloadShaders();

    void render();

    void setCurrentCamera(Camera *camera);

    void setSkybox(const Texture *texture);

    void resize(uint width, uint height);

private:
    Camera *m_camera = nullptr;

    const std::vector<DrawableUptr> &m_drawables;

    const std::vector<LightUptr> &m_lights;

    OGLShaderManager m_shaderManager;

    const OGLTexture *m_skybox = nullptr;

    OGLTextureUptr m_noiseSSAO = nullptr;

    OGLFramebuffer m_GBuffer{3};

    OGLFramebuffer m_ssaoFBO{1};

    OGLFramebuffer m_shadowMapFBO{1};

    OGLFramebuffer m_hdrFBO{1};

    OGLVAO m_quadVAO;

    OGLVAO m_cubeVAO;

private:
    void gbufferPass();

    void ssaoPass();

    void ssaoBlurPass();

    void lightingPass();

    void tonemappingPass();

    void skyboxPass();

    void drawLight(Light *light);

    void drawShadowMap(Light *light);

    void traverseGraph(OGLShader *shader, const SceneBVHNode *node) const;

    void drawAABB(OGLShader *shader, const AABB &aabb) const;

    bool inFrustum(const AABB &aabb, const Mat4 &projection) const;

    void initFBOs();

    void setupTextureUnits();
};

} // namespace AVLIT