#pragma once

#include <vector>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Base/include/Drawable.hpp>
#include <Core/Base/include/Camera.hpp>
#include <Core/Base/include/Light.hpp>
#include <Core/Renderer/include/RenderSystem.hpp>


namespace AVLIT {

class Scene {
public:
    Scene();
    ~Scene() = default;

    Scene(const Scene &) = delete;
    void operator=(const Scene &) = delete;

    AVLIT_API inline void render();

    AVLIT_API inline const std::vector<LightUptr> &lights() const;

    AVLIT_API inline const std::vector<CameraUptr> &cameras() const;

    AVLIT_API inline void setCurrentCamera(Camera *camera);

    AVLIT_API inline SceneBVHNode *graphRoot() const;

    AVLIT_API inline void resizeCanvas(int width, int height);

    AVLIT_API SceneBVHNode *addDrawable(SceneBVHNode *parent, const std::string &name, const Transform &transform,
                                       Model *model);

    AVLIT_API SceneBVHNode *addLight(SceneBVHNode *parent, const std::string &name, const Transform &transform,
                                    LightType type, const Color3 &color,
                                    float innerAngle = pi() / 8, float outerAngle = pi() / 6);

    AVLIT_API SceneBVHNode *addCamera(SceneBVHNode *parent, const std::string &name, const Transform &transform,
                                     int width, int height, float fovy, float near = 0.75f, float far = 250.f);

    AVLIT_API void reloadShaders();

    AVLIT_API void setAmbientColor(const Color3 &color);

    AVLIT_API void setSkybox(const Texture *texture);

    AVLIT_API void deleteGraphNode(SceneBVHNode *node);

private:
    SceneBVHNodeUptr m_graphRoot;

    RenderSystemUptr m_renderSystem = nullptr;

    // TODO: handle deletion of scene objects from the graph
    std::vector<SceneBVHNodeUptr> m_nodes;
    std::vector<DrawableUptr> m_drawables;
    std::vector<LightUptr> m_lights;
    std::vector<CameraUptr> m_cameras;
};

} // namespace AVLIT

#include <Core/Base/inline/Scene.inl>