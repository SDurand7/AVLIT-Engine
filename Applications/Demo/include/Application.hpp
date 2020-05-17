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

    ALIT::SceneBVHNode *addCamera(const std::string &name, const ALIT::Transform &transform) const;

    ALIT::SceneBVHNode *addDrawable(const std::string &name, const ALIT::Transform &transform);

    ALIT::SceneBVHNode *addLight(ALIT::LightType type, const ALIT::Transform &transform,
                                 const ALIT::Color3 &color = ALIT::Color3(0.6f));

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

    ALIT::Scene *scene() const;

    void setCamera(ALIT::Camera *camera);

private:
    ALIT::Scene *m_scene;
    ALIT::Camera *m_camera;

    // Used to create new cameras
    static const float m_defaultFovy;
    int m_width;
    int m_height;
};
