#include "OGLRenderSystem.hpp"

#include <random>

#include <Core/Base/include/Math.hpp>
#include <Core/Base/include/Drawable.hpp>
#include <Core/Base/include/Light.hpp>
#include <Core/Base/include/Model.hpp>
#include <Core/Base/include/Mesh.hpp>
#include <Core/Base/include/Material.hpp>


namespace ALIT {

OGLRenderSystem::OGLRenderSystem(const std::vector<DrawableUptr> &drawables, const std::vector<LightUptr> &lights,
                                 const SceneBVHNode *graphRoot)
    : m_drawables{drawables}, m_lights{lights}, m_graphRoot{graphRoot} {
    if(!gladLoadGL()) {
        LOG("[ERROR]: GLAD could not load OpenGL's functions");
    }

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
}

void OGLRenderSystem::reloadShaders() {
    m_shaderManager = OGLShaderManager{{{"GBuffer.vert.glsl", "GBuffer.frag.glsl"},
                                        {"DeferredLighting.vert.glsl", "DeferredLighting.frag.glsl"},
                                        {"ShadowMapping.vert.glsl", "ShadowMapping.frag.glsl"},
                                        {"Skybox.vert.glsl", "Skybox.frag.glsl"},
                                        {"Tonemapping.vert.glsl", "Tonemapping.frag.glsl"},
                                        {"SSAO.vert.glsl", "SSAO.frag.glsl"},
                                        {"Blur.vert.glsl", "Blur.frag.glsl"},
                                        {"Debug.vert.glsl", "Debug.frag.glsl"}}};

    if(m_camera) {
        setupTextureUnits();
    }
}

/// Deferred shading
void OGLRenderSystem::render() {
    if(m_camera) {
        gbufferPass();

        ssaoPass();

        ssaoBlurPass();

        lightingPass();

        tonemappingPass();

        skyboxPass();

        OGLVAO::unbindAll();
    }
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
            shader->setUniform(0, m_camera->projection());
            shader->setUniform(1, modelView);
            shader->setUniform(2, drawable->transform());
            const auto model = drawable->model();
            for(const auto &meshByMaterial : model->meshesByMaterial()) {
                meshByMaterial.first->isTwoSided() ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);
                meshByMaterial.first->setParameters(3, shader);
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
    m_GBuffer.blitDepthBuffer(m_camera->width(), m_camera->height());
}

void OGLRenderSystem::ssaoPass() {
    m_ssaoFBO.bind();
    auto shader = m_shaderManager.shader(OGLShaderType::SSAO);
    shader->bind();

    glDisable(GL_DEPTH_TEST);
    m_quadVAO.bind();

    shader->setUniform(2, m_camera->view());
    shader->setUniform(3, m_camera->projection());
    shader->setUniform(4, m_camera->inverseProjection());

    shader->setUniform(5, m_camera->width());
    shader->setUniform(6, m_camera->height());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void OGLRenderSystem::ssaoBlurPass() {
    m_blurFBO.bind();
    auto shader = m_shaderManager.shader(OGLShaderType::BLUR);
    shader->bind();
    m_quadVAO.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void OGLRenderSystem::lightingPass() {
    auto shader = m_shaderManager.shader(OGLShaderType::DEFERRED_LIGHTING);
    shader->bind();
    shader->setUniform(10, m_camera->transform());
    shader->setUniform(11, m_camera->inverseProjection());
    m_shadowMap.bindBuffer(12, 0, 0);
    m_shadowMap.bindBuffer(13, 1, 0);

    if(m_skybox.hasTexture())
        m_skybox.bind(8, shader, 0);

    // Ambient light
    m_hdrFBO.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    m_quadVAO.bind();
    m_lights[0]->setParameters(12, shader);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
    auto shadowMapSize = m_shadowMap.size();

    for(int i = 1; i < static_cast<int>(m_lights.size()); ++i) {
        if(m_lights[i]->isLit()) {
            glViewport(0, 0, shadowMapSize.first, shadowMapSize.second);
            drawShadowMap(m_lights[i].get());

            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glViewport(0, 0, m_camera->width(), m_camera->height());
            drawLight(m_lights[i].get());
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);
        }
    }
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void OGLRenderSystem::tonemappingPass() {
    OGLFramebuffer::bindDefault();
    auto shader = m_shaderManager.shader(OGLShaderType::TONEMAPPING);
    shader->bind();

    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
}

void OGLRenderSystem::skyboxPass() {
    if(m_skybox.hasTexture()) {
        glDepthMask(GL_FALSE);
        auto shader = m_shaderManager.shader(OGLShaderType::SKYBOX);
        shader->bind();

        Mat4 skyboxView = m_camera->view();
        skyboxView[3] = {0.f, 0.f, 0.f, 1.f};
        shader->setUniform(0, m_camera->projection() * skyboxView);
        m_skybox.bind(1, shader, 0);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glDepthMask(GL_TRUE);
    }
}

void OGLRenderSystem::drawLight(Light *light) {
    auto shader = m_shaderManager.shader(OGLShaderType::DEFERRED_LIGHTING);
    m_hdrFBO.bind();
    shader->bind();
    shader->setUniform(9, light->projection() * light->view());
    m_quadVAO.bind();
    light->setParameters(12, shader);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void OGLRenderSystem::drawShadowMap(Light *light) {
    auto shader = m_shaderManager.shader(OGLShaderType::SHADOW_MAPPING);
    shader->bind();
    m_shadowMap.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    for(const auto &drawable : m_drawables) {
        AABB aabb = drawable->aabb();
        aabb.applyTransform(light->view());
        if(drawable->isVisible() && inFrustum(aabb, light->projection())) {
            Mat4 modelView = light->view() * Mat4{drawable->transform()};
            shader->setUniform(0, light->projection() * modelView);
            const auto model = drawable->model();
            for(const auto &meshByMaterial : model->meshesByMaterial()) {
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

void OGLRenderSystem::setupTextureUnits() {
    auto shader = m_shaderManager.shader(OGLShaderType::SSAO);
    shader->bind();
    m_GBuffer.setParameters(0, shader, 5);
    m_ssaoFBO.setParameters(1, shader, 10);

    std::default_random_engine gen;
    std::uniform_real_distribution<float> distribution1{-1.f, 1.f};
    std::uniform_real_distribution<float> distribution2{0.f, 1.f};
    for(int i = 0; i < 64; ++i) {
        Vec3 sample{distribution1(gen), distribution1(gen), distribution2(gen)};

        float scale = static_cast<float>(i) / 64;
        float scaleSquared = scale * scale;
        scale = 0.1f * (1 - scaleSquared) + 1.0f * scaleSquared;
        sample = scale * normalize(sample);

        shader->setUniform(7 + i, sample);
    }

    shader = m_shaderManager.shader(OGLShaderType::BLUR);
    shader->bind();
    m_ssaoFBO.setParameters(0, shader, 11);

    shader = m_shaderManager.shader(OGLShaderType::DEFERRED_LIGHTING);
    shader->bind();
    m_GBuffer.setParameters(0, shader, 5);
    m_GBuffer.setParameters(1, shader, 6);
    m_GBuffer.setParameters(2, shader, 7);
    m_GBuffer.setParameters(3, shader, 8);
    m_GBuffer.setParameters(4, shader, 9);
    m_blurFBO.setParameters(5, shader, 15);
    m_shadowMap.setParameters(6, shader, 12);
    m_shadowMap.setParameters(7, shader, 13);

    shader = m_shaderManager.shader(OGLShaderType::TONEMAPPING);
    shader->bind();
    m_hdrFBO.setParameters(0, shader, 14);
}

} // namespace ALIT