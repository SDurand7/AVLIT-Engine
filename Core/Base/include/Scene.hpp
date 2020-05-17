#ifndef ALIT_SCENE_HPP
#define ALIT_SCENE_HPP

#include <vector>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Base/include/Drawable.hpp>
#include <Core/Base/include/Camera.hpp>
#include <Core/Base/include/Light.hpp>


namespace ALIT {

class Scene {
public:
    Scene();
    ~Scene() = default;

    Scene(const Scene &) = delete;
    void operator=(const Scene &) = delete;

    ALIT_API inline void render();

    ALIT_API inline const std::vector<LightUptr> &lights() const;

    ALIT_API inline const std::vector<CameraUptr> &cameras() const;

    ALIT_API inline void setCurrentCamera(Camera *camera);

    ALIT_API inline SceneBVHNode *graphRoot() const;

    ALIT_API inline void resize(int width, int height);

    ALIT_API SceneBVHNode *addDrawable(SceneBVHNode *parent, const std::string &name, const Transform &transform,
                                       Model *model);

    ALIT_API SceneBVHNode *addLight(SceneBVHNode *parent, const std::string &name, const Transform &transform,
                                    LightType type, const Color3 &color, float range = 30.f,
                                    float innerAngle = pi() / 8, float outerAngle = pi() / 6);

    ALIT_API SceneBVHNode *addCamera(SceneBVHNode *parent, const std::string &name, const Transform &transform,
                                     int width, int height, float fovy, float near = 0.4f, float far = 450.f);

    ALIT_API void reloadShaders();

    ALIT_API void setAmbientColor(const Color3 &color);

    ALIT_API void setSkybox(const Texture *texture);

    ALIT_API void deleteGraphNode(SceneBVHNode *node);

private:
    SceneBVHNodeUptr m_graphRoot;

    RenderSystemUptr m_renderSystem;

    // TODO: handle deletion of scene objects from the graph
    std::vector<SceneBVHNodeUptr> m_nodes;
    std::vector<DrawableUptr> m_drawables;
    std::vector<LightUptr> m_lights;
    std::vector<CameraUptr> m_cameras;
};

} // namespace ALIT

#include <Core/Base/inline/Scene.inl>

#endif