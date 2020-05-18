#include <Core/Renderer/include/RenderSystem.hpp>


namespace AVLIT {

inline void Scene::render() { m_renderSystem->render(); }

inline const std::vector<LightUptr> &Scene::lights() const { return m_lights; }

inline const std::vector<CameraUptr> &Scene::cameras() const { return m_cameras; }

inline void Scene::setCurrentCamera(Camera *camera) { m_renderSystem->setCurrentCamera(camera); }

inline SceneBVHNode *Scene::graphRoot() const { return m_graphRoot.get(); }

inline void Scene::resizeCanvas(int width, int height) {
    m_renderSystem->resize(width, height);

    for(auto &camera : m_cameras) {
        camera->setWidth(width);
        camera->setHeight(height);
    }
}

} // namespace AVLIT