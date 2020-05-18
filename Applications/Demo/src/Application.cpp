#include "Application.hpp"

#include <Core/Base/include/Engine.hpp>
#include <Core/Base/include/Scene.hpp>
#include <Core/Base/include/Math.hpp>
#include <Core/Base/include/Drawable.hpp>
#include <Core/Base/include/Light.hpp>
#include <Core/Base/include/AssetManager.hpp>
#include <Core/Base/include/ModelManager.hpp>


const float Application::m_defaultFovy = 1.2f;

Application::Application(int width, int height)
    : m_scene(AVLIT::Engine::instance()->scene()), m_width(width), m_height(height) {
    AVLIT::Transform transform(1.f);
    transform[3] = AVLIT::Vec3(0.f, 0.f, 5.f);
    m_camera = static_cast<AVLIT::Camera *>(addCamera("Main camera", transform)->sceneObject());
    m_scene->setCurrentCamera(m_camera);
    m_scene->setAmbientColor({0.06f, 0.04f, 0.03f});

    // Setup debug meshes
    std::string directory = AVLIT_ROOT "/Assets/";

    std::string name = loadModel(directory + "Sponza/sponza.obj");
    setSkybox({directory + "Skybox/right.jpg", directory + "Skybox/left.jpg", directory + "Skybox/top.jpg",
               directory + "Skybox/bottom.jpg", directory + "Skybox/front.jpg", directory + "Skybox/back.jpg"});

    addDrawable(name, AVLIT::Transform(0.05f));
    addLight(AVLIT::LightType::DIRECTIONAL_LIGHT, AVLIT::rotate({1.f, 0.f, 0.f}, -AVLIT::pi() / 3), {0.8f, 0.65f, 0.42f});

    transform = AVLIT::rotate({0.f, 1.f, 0.f}, AVLIT::pi() / 2);
    transform[3] += AVLIT::Vec3(-45.f, 4.f, -1.f);
    addLight(AVLIT::LightType::SPOT_LIGHT, transform, {8.f, 6.f, 4.5f});
}

void Application::resize(int width, int height) {
    m_width = width;
    m_height = height;

    m_camera->setWidth(width);
    m_camera->setHeight(height);
}

void Application::translateCameraFront(float distance) { m_camera->translate(-distance * m_camera->front()); }

void Application::translateCameraRight(float distance) { m_camera->translate(distance * m_camera->right()); }

void Application::rotateCamera(int dx, int dy) {
    float angleX = static_cast<float>(dx) / m_width * m_camera->fovx();
    float angleY = static_cast<float>(dy) / m_height * m_camera->fovy();

    m_camera->rotate(AVLIT::rotate(AVLIT::Vec3(0.f, 1.f, 0.f), angleX) * AVLIT::rotate(m_camera->right(), angleY),
                     m_camera->position());
}

AVLIT::SceneBVHNode *Application::addCamera(const std::string &name, const AVLIT::Transform &transform) const {
    return m_scene->addCamera(m_scene->graphRoot(), name, transform, m_width, m_height, m_defaultFovy);
}

AVLIT::SceneBVHNode *Application::addDrawable(const std::string &name, const AVLIT::Transform &transform) {
    return m_scene->addDrawable(m_scene->graphRoot(), name, transform,
                                AVLIT::Engine::instance()->assetManager()->modelManager()->model(name));
}

AVLIT::SceneBVHNode *Application::addLight(AVLIT::LightType type, const AVLIT::Transform &transform,
                                          const AVLIT::Color3 &color) {
    std::string name = (type == AVLIT::LightType::DIRECTIONAL_LIGHT
                            ? "New directional light"
                            : (type == AVLIT::LightType::SPOT_LIGHT ? "New spot light" : "New point light"));
    return m_scene->addLight(m_scene->graphRoot(), name, transform, type, color);
}

std::string Application::loadModel(const std::string &filename) {
    auto assetManager = AVLIT::Engine::instance()->assetManager();
    return assetManager->loadAssetFromFile(filename);
}

void Application::setSkybox(const std::vector<std::string> &filenames) {
    m_scene->setSkybox(AVLIT::Engine::instance()->assetManager()->loadSkyboxTexture(filenames));
}

void Application::reloadShaders() { m_scene->reloadShaders(); }

AVLIT::Scene *Application::scene() const { return m_scene; }
