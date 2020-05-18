#include "Scene.hpp"

#include <Core/Base/include/SceneBVHNode.hpp>

#include <algorithm>


namespace AVLIT {

Scene::Scene()
    : m_graphRoot{std::make_unique<SceneBVHNode>(nullptr)}, m_renderSystem{std::make_unique<RenderSystem>(
                                                                m_drawables, m_lights, m_graphRoot.get())},
      m_drawables{}, m_lights{}, m_cameras{} {
    Transform trf{1.f};
    trf[3] = {0.f, 10.f, 0.f};
    m_lights.emplace_back(std::make_unique<AmbientLight>("Ambient light", trf, Color3{0.f}));

    Light *light = m_lights.back().get();

    m_nodes.emplace_back(std::make_unique<SceneBVHNode>(light));

    auto child = m_nodes.back().get();
    m_graphRoot->addChild(child);
    light->setGraphNode(child);
}

SceneBVHNode *Scene::addDrawable(SceneBVHNode *parent, const std::string &name, const Transform &transform,
                                 Model *model) {
    m_drawables.emplace_back(std::make_unique<Drawable>(name, transform, model));
    Drawable *drawable = m_drawables.back().get();

    m_nodes.emplace_back(std::make_unique<SceneBVHNode>(drawable));

    auto child = m_nodes.back().get();

    parent->addChild(child);
    drawable->setGraphNode(child);

    return child;
}

SceneBVHNode *Scene::addLight(SceneBVHNode *parent, const std::string &name, const Transform &transform, LightType type,
                              const Color3 &color, float innerAngle, float outerAngle) {
    switch(type) {
    case LightType::POINT_LIGHT:
        m_lights.emplace_back(std::make_unique<PointLight>(name, transform, color));
        break;
    case LightType::DIRECTIONAL_LIGHT:
        m_lights.emplace_back(std::make_unique<DirectionalLight>(name, transform, color));
        break;
    case LightType::SPOT_LIGHT:
        m_lights.emplace_back(std::make_unique<SpotLight>(name, transform, color, innerAngle, outerAngle));
        break;
    default:
        return nullptr;
    }

    Light *light = m_lights.back().get();

    m_nodes.emplace_back(std::make_unique<SceneBVHNode>(light));

    auto child = m_nodes.back().get();

    parent->addChild(child);
    light->setGraphNode(child);

    return child;
}

SceneBVHNode *Scene::addCamera(SceneBVHNode *parent, const std::string &name, const Transform &transform, int width,
                               int height, float fovy, float near, float far) {
    m_cameras.emplace_back(std::make_unique<Camera>(name, transform, width, height, fovy, near, far));
    Camera *camera = m_cameras.back().get();

    m_nodes.emplace_back(std::make_unique<SceneBVHNode>(camera));

    auto child = m_nodes.back().get();

    parent->addChild(child);
    camera->setGraphNode(child);

    return child;
}

void Scene::reloadShaders() { m_renderSystem->reloadShaders(); }

void Scene::setAmbientColor(const Color3 &color) { m_lights[0]->setColor(color); }

void Scene::setSkybox(const Texture *texture) { m_renderSystem->setSkybox(texture); }

void Scene::deleteGraphNode(SceneBVHNode *node) {
    AVLIT_ASSERT((std::find_if(m_nodes.begin(), m_nodes.end(),
                               [node](const SceneBVHNodeUptr &p) { return p.get() == node; }) != m_nodes.end()),
                 "[ASSERTION FAILED]: Specified node isn't part of the graph");

    m_drawables.erase(
        std::remove_if(m_drawables.begin(), m_drawables.end(),
                       [object = node->sceneObject()](const DrawableUptr &p) { return p.get() == object; }),
        m_drawables.end());

    m_lights.erase(std::remove_if(m_lights.begin(), m_lights.end(),
                                  [object = node->sceneObject()](const LightUptr &p) { return p.get() == object; }),
                   m_lights.end());

    m_cameras.erase(std::remove_if(m_cameras.begin(), m_cameras.end(),
                                   [object = node->sceneObject()](const CameraUptr &p) { return p.get() == object; }),
                    m_cameras.end());

    m_nodes.erase(
        std::remove_if(m_nodes.begin(), m_nodes.end(), [node](const SceneBVHNodeUptr &p) { return p.get() == node; }),
        m_nodes.end());
}

} // namespace AVLIT