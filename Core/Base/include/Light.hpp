#pragma once

#include <Core/Base/include/SceneObject.hpp>


namespace AVLIT {

class Light : public SceneObject {
public:
    // transform should be a composition of a rotation and a translation, debug models should be scaled on load
    Light(const std::string &name, const Transform &transform, const Color3 &color);

    virtual ~Light() = default;

    virtual void setParameters(const std::string& name, Shader *shader) const = 0;

    virtual const Mat4 &projection() const = 0;

    inline Mat4 view() const;

    AVLIT_API inline Color3 color() const;

    AVLIT_API inline void setColor(const Color3 &color);

    AVLIT_API inline bool isLit() const;

    AVLIT_API inline void switchState();

protected:
    Color3 m_color;

    bool m_lit = true;
};

class PointLight : public Light {
public:
    PointLight(const std::string &name, const Transform &transform, const Color3 &color);

    void setParameters(const std::string &name, Shader *shader) const override;

    const Mat4 &projection() const override;

private:
    static const float maxRange;
    static const Mat4 m_projection;
};

class DirectionalLight : public Light {
public:
    DirectionalLight(const std::string &name, const Transform &transform, const Color3 &color);

    void setParameters(const std::string &name, Shader *shader) const override;

    const Mat4 &projection() const override;

private:
    static const Mat4 m_projection;
};

class SpotLight : public Light {
public:
    SpotLight(const std::string &name, const Transform &transform, const Color3 &color, float innerAngle,
              float outerAngle);

    void setParameters(const std::string &name, Shader *shader) const override;

    const Mat4 &projection() const override;

    AVLIT_API float innerAngle() const;

    AVLIT_API void setInnerAngle(float angle);

    AVLIT_API float outerAngle() const;

    AVLIT_API void setOuterAngle(float angle);

private:
    static const float maxRange;
    static const Mat4 m_projection;

    float m_cosInnerAngle;
    float m_cosOuterAngle;
};

class AmbientLight : public Light {
public:
    AmbientLight(const std::string &name, const Transform &transform, const Color3 &color);

    void setParameters(const std::string &name, Shader *shader) const override;

    const Mat4 &projection() const override;

private:
    static const Mat4 m_projection;
};

} // namespace AVLIT

#include <Core/Base/inline/Light.inl>