#ifndef ALIT_ENGINE_HPP
#define ALIT_ENGINE_HPP

#ifdef DX_

#else
#include <Core/Renderer/OpenGL/include/glad.h>
#endif

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>


namespace ALIT {

class Engine {
public:
    Engine(const Engine &) = delete;
    void operator=(const Engine &) = delete;

    // Must be called after the creation of the OpenGL context
    ALIT_API static void createInstance();

    ALIT_API static Engine *instance();

    ALIT_API static void deleteInstance();

    ALIT_API Scene *scene() const;

    ALIT_API AssetManager *assetManager() const;

private:
    Engine();
    
    static Engine *m_instance;

    SceneUptr m_scene;
    AssetManagerUptr m_assetManager;
    // ...
};

} // namespace ALIT

#endif