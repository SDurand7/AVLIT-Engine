#pragma once

#ifdef DX_

#else
#include <Core/Renderer/OpenGL/include/glad.h>
#endif

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>


namespace AVLIT {

class Engine {
public:
    Engine(const Engine &) = delete;
    void operator=(const Engine &) = delete;

    // Must be called after the creation of the OpenGL context
    AVLIT_API static void createInstance();

    AVLIT_API static Engine *instance();

    AVLIT_API static void deleteInstance();

    AVLIT_API Scene *scene() const;

    AVLIT_API AssetManager *assetManager() const;

private:
    Engine();

    static Engine *m_instance;

    SceneUptr m_scene;
    AssetManagerUptr m_assetManager;
    // ...
};

} // namespace AVLIT