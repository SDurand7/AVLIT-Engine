#include "OGLRenderSystem.hpp"

#include <random>

#include <Core/Base/include/Math.hpp>
#include <Core/Base/include/Drawable.hpp>
#include <Core/Base/include/Light.hpp>
#include <Core/Base/include/Model.hpp>
#include <Core/Base/include/Mesh.hpp>
#include <Core/Base/include/Material.hpp>
#include <Core/Renderer/OpenGL/include/OGLTexture.hpp>

#include "bluenoise.h"


namespace AVLIT {

static constexpr GLuint ShadowMapResolution = 2048;

void OGLRenderSystem::initGL() {
    if(!gladLoadGL())
        AVLIT_ERROR("GLAD could not load OpenGL's functions");
}

OGLRenderSystem::OGLRenderSystem(const std::vector<DrawableUptr> &drawables, const std::vector<LightUptr> &lights,
                                 Camera *camera)
    : m_camera{camera}, m_drawables{drawables}, m_lights{lights} {
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_ONE, GL_ONE);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(.85f, .75f);

    reloadShaders();

    m_quadVAO = OGLVAO{
        Mesh{nullptr, {0, 1, 2, 0, 3, 1}, {{-1.f, -1.f, 0.f}, {1.f, 1.f, 0.f}, {-1.f, 1.f, 0.f}, {1.f, -1.f, 0.f}}}};

    m_cubeVAO = OGLVAO{Mesh{
        nullptr,
        {0, 1, 3, 3, 1, 2, 1, 5, 2, 2, 5, 6, 5, 4, 6, 6, 4, 7, 4, 0, 7, 7, 0, 3, 3, 2, 7, 7, 2, 6, 4, 5, 0, 0, 5, 1},
        {{-1.f, -1.f, -1.f},
         {1.f, -1.f, -1.f},
         {1.f, 1.f, -1.f},
         {-1.f, 1.f, -1.f},
         {-1.f, -1.f, 1.f},
         {1.f, -1.f, 1.f},
         {1.f, 1.f, 1.f},
         {-1.f, 1.f, 1.f}}}};

    OGLFramebuffer::saveDefault();

    // Create the SSAO noise texture // TODO: center & normalize
    std::vector<float> tile(std::begin(bluenoise), std::end(bluenoise));
    float *data = tile.data();
    m_noiseSSAO = std::make_unique<OGLTexture>(TextureInternalFormat::RG32F, TextureFormat::RG, TextureType::TEXTURE2D,
                                               TextureDataType::FLOAT, false);
    m_noiseSSAO->bind(9);
    m_noiseSSAO->allocate<float>(4, 4, &data);
    m_noiseSSAO->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_noiseSSAO->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_noiseSSAO->setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    m_noiseSSAO->setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

    initFBOs();
    setupTextureUnits();

    OGLFramebuffer::bindDefault();

    GL_CHECK_ERROR();
}

void OGLRenderSystem::reloadShaders() {
    m_shaderManager = OGLShaderManager(
        {{OGLShaderType::GBUFFER,
          {{OGLShaderStage::VERTEX, "GBuffer.vert"}, {OGLShaderStage::FRAGMENT, "GBuffer.frag"}}},
         {OGLShaderType::DEFERRED_LIGHTING,
          {{OGLShaderStage::VERTEX, "DeferredLighting.vert"}, {OGLShaderStage::FRAGMENT, "DeferredLighting.frag"}}},
         {OGLShaderType::SHADOW_MAPPING,
          {{OGLShaderStage::VERTEX, "ShadowMapping.vert"}, {OGLShaderStage::FRAGMENT, "ShadowMapping.frag"}}},
         {OGLShaderType::SKYBOX, {{OGLShaderStage::VERTEX, "Skybox.vert"}, {OGLShaderStage::FRAGMENT, "Skybox.frag"}}},
         {OGLShaderType::TONEMAPPING,
          {{OGLShaderStage::VERTEX, "Tonemapping.vert"}, {OGLShaderStage::FRAGMENT, "Tonemapping.frag"}}},
         {OGLShaderType::SSAO, {{OGLShaderStage::VERTEX, "SSAO.vert"}, {OGLShaderStage::FRAGMENT, "SSAO.frag"}}}});

    if(m_camera) {
        setupTextureUnits();
    }

    GL_CHECK_ERROR();

    AVLIT_LOG("shaders reload complete");
}

/// Deferred shading
void OGLRenderSystem::render() {
    if(m_camera) {
        gbufferPass();

        ssaoPass();

        lightingPass();

        tonemappingPass();

        skyboxPass();

        OGLVAO::unbindAll();
    }

    GL_CHECK_ERROR();
}

void OGLRenderSystem::setCurrentCamera(Camera *camera) { m_camera = camera; }

void OGLRenderSystem::setSkybox(const Texture *texture) {
    m_skybox = texture;

    auto shader = m_shaderManager.shader(OGLShaderType::SKYBOX);
    shader->bind();

    m_skybox->bind(8);
    shader->setUniform("skybox", 8);
}

void OGLRenderSystem::resize(uint width, uint height) {
    OGLFramebuffer::saveDefault();

    OGLTexture *texture;

    m_GBuffer.resizeRBO(GL_DEPTH24_STENCIL8, width, height);
    texture = m_GBuffer.texture("normalZMap");
    texture->bind(11);
    texture->allocate<nullptr_t>(width, height, nullptr);

    texture = m_GBuffer.texture("albedoMap");
    texture->bind(12);
    texture->allocate<nullptr_t>(width, height, nullptr);

    texture = m_GBuffer.texture("metalnessRoughnessMap");
    texture->bind(13);
    texture->allocate<nullptr_t>(width, height, nullptr);

    texture = m_ssaoFBO.texture("occlusionMap");
    texture->bind(14);
    texture->allocate<nullptr_t>(width, height, nullptr);

    texture = m_hdrFBO.texture("hdrImage");
    texture->bind(15);
    texture->allocate<nullptr_t>(width, height, nullptr);

    OGLFramebuffer::bindDefault();
}


void OGLRenderSystem::gbufferPass() {
    OGLFramebuffer::saveDefault();
    auto shader = m_shaderManager.shader(OGLShaderType::GBUFFER);
    shader->bind();
    m_GBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// GBuffer pass
    for(const auto &drawable : m_drawables) {
        AABB aabb = drawable->aabb();
        aabb.applyTransform(m_camera->view());

        if(drawable->isVisible() && inFrustum(aabb, m_camera->projection())) {
            Mat4 modelView = m_camera->view() * Mat4{drawable->transform()};
            shader->setUniform("projection", m_camera->projection());
            shader->setUniform("modelView", modelView);
            shader->setUniform("model", drawable->transform());

            const auto model = drawable->model();
            for(const auto &meshByMaterial : model->meshesByMaterial()) {
                meshByMaterial.first->isTwoSided() ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);
                meshByMaterial.first->setParameters("material", shader);
                for(const auto mesh : meshByMaterial.second) {
                    aabb = mesh->aabb();
                    aabb.applyTransform(modelView);
                    if(inFrustum(aabb, m_camera->projection())) {
                        mesh->buffer().bind();
                        glDrawElements(GL_TRIANGLES, mesh->size(), GL_UNSIGNED_INT, nullptr);
                    }
                }
            }
        }
    }

    m_GBuffer.copyDepthBuffer(m_camera->width(), m_camera->height(), OGLFramebuffer::defaultID());
    GL_CHECK_ERROR();
}

void OGLRenderSystem::ssaoPass() {
    m_ssaoFBO.bind();

    auto shader = m_shaderManager.shader(OGLShaderType::SSAO);
    shader->bind();
    shader->setUniform("view", m_camera->view());
    shader->setUniform("projection", m_camera->projection());
    shader->setUniform("inverseProjection", m_camera->inverseProjection());
    shader->setUniform("width", m_camera->width());
    shader->setUniform("height", m_camera->height());

    m_quadVAO.bind();

    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    GL_CHECK_ERROR();
}

void OGLRenderSystem::lightingPass() {
    auto shader = m_shaderManager.shader(OGLShaderType::DEFERRED_LIGHTING);
    shader->bind();
    shader->setUniform("inverseView", m_camera->transform());
    shader->setUniform("inverseProjection", m_camera->inverseProjection());    

    // Ambient light
    m_hdrFBO.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    m_quadVAO.bind();
    m_lights[0]->setParameters("light", shader);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);

    for(int i = 1; i < static_cast<int>(m_lights.size()); ++i) {
        if(m_lights[i]->isLit()) {
            glViewport(0, 0, ShadowMapResolution, ShadowMapResolution);
            drawShadowMap(m_lights[i].get());

            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);

            glViewport(0, 0, m_camera->width(), m_camera->height());
            drawLight(m_lights[i].get());

            glEnable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);
        }
    }

    GL_CHECK_ERROR();
}

void OGLRenderSystem::tonemappingPass() {
    OGLFramebuffer::bindDefault();
    auto shader = m_shaderManager.shader(OGLShaderType::TONEMAPPING);
    shader->bind();

    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);

    GL_CHECK_ERROR();
}

void OGLRenderSystem::skyboxPass() {
    if(m_skybox) {
        glDepthMask(GL_FALSE);
        auto shader = m_shaderManager.shader(OGLShaderType::SKYBOX);
        shader->bind();

        Mat4 skyboxView = m_camera->view();
        skyboxView[3] = {0.f, 0.f, 0.f, 1.f};
        shader->setUniform("viewProjection", m_camera->projection() * skyboxView);
        m_cubeVAO.bind();

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glDepthMask(GL_TRUE);

        GL_CHECK_ERROR();
    }
}

void OGLRenderSystem::drawLight(Light *light) {
    auto shader = m_shaderManager.shader(OGLShaderType::DEFERRED_LIGHTING);
    shader->bind();
    m_hdrFBO.bind();
    shader->setUniform("lightVP", light->projection() * light->view());
    m_quadVAO.bind();
    light->setParameters("light", shader);
    m_shadowMapFBO.texture("shadowMap")->bind(10);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    GL_CHECK_ERROR();
}

void OGLRenderSystem::drawShadowMap(Light *light) {
    auto shader = m_shaderManager.shader(OGLShaderType::SHADOW_MAPPING);
    shader->bind();
    m_shadowMapFBO.bind();

    glClear(GL_DEPTH_BUFFER_BIT);
    for(const auto &drawable : m_drawables) {
        AABB aabb = drawable->aabb();
        aabb.applyTransform(light->view());
        if(drawable->isVisible() && inFrustum(aabb, light->projection())) {
            Mat4 modelView = light->view() * Mat4{drawable->transform()};
            shader->setUniform("mvp", light->projection() * modelView);

            const auto model = drawable->model();
            for(const auto &meshByMaterial : model->meshesByMaterial()) {
                auto alphaMap = meshByMaterial.first->alphaMap();

                shader->setUniform("hasAlphaMap", static_cast<bool>(alphaMap));
                if(alphaMap) {
                    alphaMap->bind(0);
                    shader->setUniform("alphaMap", 0);
                } else {
                    shader->setUniform("alpha", meshByMaterial.first->alpha());
                }

                for(const auto mesh : meshByMaterial.second) {
                    aabb = mesh->aabb();
                    aabb.applyTransform(modelView);
                    if(inFrustum(aabb, light->projection())) {
                        mesh->buffer().bind();
                        glDrawElements(GL_TRIANGLES, mesh->size(), GL_UNSIGNED_INT, nullptr);
                    }
                }
            }
        }
    }

    GL_CHECK_ERROR();
}

// Based on:
// https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
bool OGLRenderSystem::inFrustum(const AABB &aabb, const Mat4 &projection) const {
    Mat4 spaceT = transpose(projection);

    Vec4 min = Vec4{aabb.min(), 1.f};
    Vec4 max = Vec4{aabb.max(), 1.f};
    float maxz = max.z;
    max.z = min.z;

    Vec4 left = spaceT[3] + spaceT[0];
    if(dot(left, max) < 0.f)
        return false;

    Vec4 right = spaceT[3] - spaceT[0];
    if(dot(right, min) < 0.f)
        return false;

    Vec4 bottom = spaceT[3] + spaceT[1];
    if(dot(bottom, max) < 0.f)
        return false;

    Vec4 top = spaceT[3] - spaceT[1];
    if(dot(top, min) < 0.f)
        return false;

    Vec4 near = spaceT[3] + spaceT[2];
    if(dot(near, max) < 0.f)
        return false;

    min.z = maxz;
    Vec4 far = spaceT[3] - spaceT[2];
    if(dot(far, min) < 0.f)
        return false;

    return true;
}

void OGLRenderSystem::initFBOs() {
    const GLuint width = m_camera->width();
    const GLuint height = m_camera->height();

    std::vector<GLenum> attachments{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    std::unique_ptr<Texture> texture = nullptr;

    // Shadow Map FBO
    m_shadowMapFBO.bind();
    texture = std::make_unique<OGLTexture>(TextureInternalFormat::DEPTH, TextureFormat::DEPTH, TextureType::TEXTURE2D,
                                           TextureDataType::FLOAT, false);
    texture->bind(10);
    texture->allocate<nullptr_t>(ShadowMapResolution, ShadowMapResolution, nullptr);
    texture->setParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    m_shadowMapFBO.attachTexture("shadowMap", std::move(texture), GL_DEPTH_ATTACHMENT);
    m_shadowMapFBO.setDrawBuffer(GL_NONE);
    m_shadowMapFBO.setReadBuffer(GL_NONE);
    if(!m_shadowMapFBO.isComplete())
        AVLIT_ERROR("incomplete shadow map FBO");

    // GBuffer FBO
    m_GBuffer.bind();
    texture = std::make_unique<OGLTexture>(TextureInternalFormat::RGBA32F, TextureFormat::RGBA, TextureType::TEXTURE2D,
                                           TextureDataType::FLOAT, false);
    texture->bind(11);
    texture->allocate<nullptr_t>(width, height, nullptr);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_GBuffer.attachTexture("normalZMap", std::move(texture), attachments[0]);

    texture = std::make_unique<Texture>(TextureInternalFormat::RGBA, TextureFormat::RGBA, TextureType::TEXTURE2D,
                                        TextureDataType::UBYTE, false);
    texture->bind(12);
    texture->allocate<nullptr_t>(width, height, nullptr);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_GBuffer.attachTexture("albedoMap", std::move(texture), attachments[1]);

    texture = std::make_unique<OGLTexture>(TextureInternalFormat::RG, TextureFormat::RG, TextureType::TEXTURE2D,
                                           TextureDataType::UBYTE, false);
    texture->bind(13);
    texture->allocate<nullptr_t>(width, height, nullptr);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_GBuffer.attachTexture("metalnessRoughnessMap", std::move(texture), attachments[2]);

    m_GBuffer.attachRenderBuffer(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, width, height);
    m_GBuffer.setDrawBuffers(attachments);
    if(!m_GBuffer.isComplete())
        AVLIT_ERROR("incomplete G-buffer FBO");

    // SSAO FBO
    m_ssaoFBO.bind();
    texture = std::make_unique<OGLTexture>(TextureInternalFormat::R32F, TextureFormat::R, TextureType::TEXTURE2D,
                                           TextureDataType::FLOAT, false);
    texture->bind(14);
    texture->allocate<nullptr_t>(width, height, nullptr);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_ssaoFBO.attachTexture("occlusionMap", std::move(texture), attachments[0]);
    m_ssaoFBO.setDrawBuffers(attachments);
    if(!m_ssaoFBO.isComplete())
        AVLIT_ERROR("incomplete SSAO FBO");

    // HDR FBO
    m_hdrFBO.bind();
    texture = std::make_unique<OGLTexture>(TextureInternalFormat::RGB32F, TextureFormat::RGBA, TextureType::TEXTURE2D,
                                           TextureDataType::FLOAT, false);
    texture->bind(15);
    texture->allocate<nullptr_t>(width, height, nullptr);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_hdrFBO.attachTexture("hdrImage", std::move(texture), attachments[0]);
    m_hdrFBO.setDrawBuffers(attachments);
    if(!m_hdrFBO.isComplete())
        AVLIT_ERROR("incomplete HDR FBO");
}

void OGLRenderSystem::setupTextureUnits() {
    auto shader = m_shaderManager.shader(OGLShaderType::SSAO);
    shader->bind();
    shader->setUniform("normalZMap", 11);
    shader->setUniform("noise", 9);

    std::mt19937 gen;
    std::uniform_real_distribution<float> distribution1{-1.f, 1.f};
    std::uniform_real_distribution<float> distribution2{0.f, 1.f};
    for(int i = 0; i < 64; ++i) {
        Vec3 sample{distribution1(gen), distribution1(gen), distribution2(gen)};

        float scale = static_cast<float>(i) / 64;
        float scaleSquared = scale * scale;
        scale = 0.1f * (1 - scaleSquared) + 1.0f * scaleSquared;
        sample = scale * normalize(sample);

        shader->setUniform("samples[" + std::to_string(i) + "]", sample);
    }

    shader = m_shaderManager.shader(OGLShaderType::DEFERRED_LIGHTING);
    shader->bind();
    shader->setUniform("shadowMap", 10);
    shader->setUniform("normalZMap", 11);
    shader->setUniform("albedoMap", 12);
    shader->setUniform("metalnessRoughnessMap", 13);
    shader->setUniform("occlusionMap", 14);

    shader = m_shaderManager.shader(OGLShaderType::TONEMAPPING);
    shader->bind();
    shader->setUniform("hdrImage", 15);

    GL_CHECK_ERROR();
}

} // namespace AVLIT