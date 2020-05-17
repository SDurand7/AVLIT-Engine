#include "Engine.hpp"

#include <Core/Base/include/Scene.hpp>
#include <Core/Base/include/AssetManager.hpp>
#include <Core/Base/include/ModelManager.hpp>
#include <Core/Base/include/MeshManager.hpp>
#include <Core/Base/include/MaterialManager.hpp>
#include <Core/Base/include/TextureManager.hpp>


namespace ALIT {

Engine *Engine::m_instance = nullptr;

Engine::Engine() {
    // Initializing sub-systems
    m_scene = std::make_unique<Scene>();
    m_assetManager = std::make_unique<AssetManager>();
}

void Engine::createInstance() {
    if(m_instance) {
        LOG("[WARNING]: Engine already created");
    } else {
        m_instance = new Engine();
    }
}
Engine *Engine::instance() {
    if(!m_instance) {
        LOG("[OOPS]: Engine was never initialiazed, segfault incoming");
    }
    return m_instance;
}

void Engine::deleteInstance() { delete m_instance; }

Scene *Engine::scene() const { return m_scene.get(); }

AssetManager *Engine::assetManager() const { return m_assetManager.get(); }

} // namespace ALIT