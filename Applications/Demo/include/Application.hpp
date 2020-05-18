#pragma once

#include <vector>
#include <string>

#include <Core/Base/include/Types.hpp>


class Application {
public:
    Application() = delete;

    Application(int width, int height);

    void resize(int width, int height);

    void translateCameraFront(float distance);

    void translateCameraRight(float distance);

    void rotateCamera(int dx, int dy);

    void useEditorCamera();

    AVLIT::SceneBVHNode *addCamera(const std::string &name, const AVLIT::Transform &transform) const;

    AVLIT::SceneBVHNode *addDrawable(const std::string &name, const AVLIT::Transform &transform);

    AVLIT::SceneBVHNode *addLight(AVLIT::LightType type, const AVLIT::Transform &transform,
                                 const AVLIT::Color3 &color = AVLIT::Color3(0.6f));

    std::string loadModel(const std::string &filename);

    void setSkybox(const std::vector<std::string> &filenames);

    void reloadShaders();

    void setTranslateGizmos();

    void setRotateGizmos();

    void setScaleGizmos();

    void setTransformSpaceWorld();

    void setTransformSpaceLocal();

    void setShowDebug(bool showDebug);

    void setShowAABBs(bool showAABBs);

    AVLIT::Scene *scene() const;

    void setCamera(AVLIT::Camera *camera);

private:
    AVLIT::Scene *m_scene;
    AVLIT::Camera *m_camera;

    // Used to create new cameras
    static const float m_defaultFovy;
    int m_width;
    int m_height;
};
