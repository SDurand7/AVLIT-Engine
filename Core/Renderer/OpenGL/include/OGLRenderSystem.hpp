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
class OGLRenderSystem {
public:
    OGLRenderSystem(const std::vector<DrawableUptr> &drawables, const std::vector<LightUptr> &lights);

    void reloadShaders();

    void render();

    inline void setShowAABBs(bool showAABBs);

    inline void setCurrentCamera(Camera *camera);

    inline void setSkybox(const Texture *texture);

    inline void resize(int width, int height);

private:
    Camera *m_camera = nullptr;

    const std::vector<DrawableUptr> &m_drawables;

    const std::vector<LightUptr> &m_lights;

    OGLShaderManager m_shaderManager;

    const OGLTexture *m_skybox = nullptr;

    OGLFramebuffer m_GBuffer;

    OGLFramebuffer m_shadowMap;

    OGLFramebuffer m_ssaoFBO;

    OGLFramebuffer m_blurFBO;

    OGLFramebuffer m_hdrFBO;

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

    void setupTextureUnits();
};

} // namespace AVLIT

#include <Core/Renderer/OpenGL/inline/OGLRenderSystem.inl>