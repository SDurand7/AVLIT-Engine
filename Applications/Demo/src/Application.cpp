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
    : m_scene(ALIT::Engine::instance()->scene()), m_width(width), m_height(height) {
    ALIT::Transform transform(1.f);
    transform[3] = ALIT::Vec3(0.f, 0.f, 5.f);
    m_camera = static_cast<ALIT::Camera *>(addCamera("Main camera", transform)->sceneObject());
    m_scene->setCurrentCamera(m_camera);
    m_scene->setAmbientColor(ALIT::Color3(0.05f));

    // Setup debug meshes
    std::string directory = AVLIT_ROOT "/Assets/";

    std::string name = loadModel(directory + "Sponza/sponza.obj");
    setSkybox({directory + "Skybox/right.jpg", directory + "Skybox/left.jpg", directory + "Skybox/top.jpg",
               directory + "Skybox/bottom.jpg", directory + "Skybox/front.jpg", directory + "Skybox/back.jpg"});

    addDrawable(name, ALIT::Transform(0.05f));
    addLight(ALIT::LightType::DIRECTIONAL_LIGHT, ALIT::rotate({1.f, 0.f, 0.f}, -ALIT::pi() / 3));

    transform = ALIT::rotate({0.f, 1.f, 0.f}, ALIT::pi() / 2);
    transform[3] += ALIT::Vec3(-45.f, 4.f, -1.f);
    addLight(ALIT::LightType::SPOT_LIGHT, transform, {10.f, 8.f, 6.f});
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

    m_camera->rotate(ALIT::rotate(ALIT::Vec3(0.f, 1.f, 0.f), angleX) * ALIT::rotate(m_camera->right(), angleY),
                     m_camera->position());
}

ALIT::SceneBVHNode *Application::addCamera(const std::string &name, const ALIT::Transform &transform) const {
    return m_scene->addCamera(m_scene->graphRoot(), name, transform, m_width, m_height, m_defaultFovy);
}

ALIT::SceneBVHNode *Application::addDrawable(const std::string &name, const ALIT::Transform &transform) {
    return m_scene->addDrawable(m_scene->graphRoot(), name, transform,
                                ALIT::Engine::instance()->assetManager()->modelManager()->model(name));
}

ALIT::SceneBVHNode *Application::addLight(ALIT::LightType type, const ALIT::Transform &transform,
                                          const ALIT::Color3 &color) {
    std::string name = (type == ALIT::LightType::DIRECTIONAL_LIGHT
                            ? "New directional light"
                            : (type == ALIT::LightType::SPOT_LIGHT ? "New spot light" : "New point light"));
    return m_scene->addLight(m_scene->graphRoot(), name, transform, type, color);
}

std::string Application::loadModel(const std::string &filename) {
    auto assetManager = ALIT::Engine::instance()->assetManager();
    return assetManager->loadAssetFromFile(filename);
}

void Application::setSkybox(const std::vector<std::string> &filenames) {
    m_scene->setSkybox(ALIT::Engine::instance()->assetManager()->loadSkyboxTexture(filenames));
}

void Application::reloadShaders() { m_scene->reloadShaders(); }

ALIT::Scene *Application::scene() const { return m_scene; }
