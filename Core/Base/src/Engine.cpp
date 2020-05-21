#include "Engine.hpp"

#include <Core/Base/include/Scene.hpp>
#include <Core/Base/include/AssetManager.hpp>
#include <Core/Base/include/ModelManager.hpp>
#include <Core/Base/include/MeshManager.hpp>
#include <Core/Base/include/MaterialManager.hpp>
#include <Core/Base/include/TextureManager.hpp>


namespace AVLIT {

Engine *Engine::m_instance = nullptr;

Engine::Engine() {
    // Initializing sub-systems
    m_scene = std::make_unique<Scene>();
    m_assetManager = std::make_unique<AssetManager>();
}

void Engine::createInstance() {
    if(m_instance) {
        AVLIT_WARN("Engine already initialized");
    } else {
        AVLIT_LOG("Initializing the engine...");
        m_instance = new Engine();
        AVLIT_LOG("Engine initialized");
    }
}
Engine *Engine::instance() {
    if(!m_instance) {
        AVLIT_WARN("Engine is uninitialized");
    }
    return m_instance;
}

void Engine::deleteInstance() {
    delete m_instance;
    AVLIT_LOG("Engine ressources freed successfully");
}

Scene *Engine::scene() const { return m_scene.get(); }

AssetManager *Engine::assetManager() const { return m_assetManager.get(); }

} // namespace AVLIT