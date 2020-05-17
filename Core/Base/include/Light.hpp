#ifndef ALIT_LIGHT_HPP
#define ALIT_LIGHT_HPP

#include <Core/Base/include/SceneObject.hpp>


namespace ALIT {

// TODO: store the view matrix and use a dirty flag
class Light : public SceneObject {
public:
    // transform should be a composition of a rotation and a translation, debug models should be scaled on load
    Light(const std::string &name, const Transform &transform, const Color3 &color);

    virtual ~Light() = default;

    virtual void setParameters(int i, Shader *shader) const = 0;

    virtual const Mat4 &projection() const = 0;

    inline Mat4 view() const;

    ALIT_API inline Color3 color() const;

    ALIT_API inline void setColor(const Color3 &color);

    ALIT_API inline bool isLit() const;

    ALIT_API inline void switchState();

protected:
    Color3 m_color;

    bool m_lit = true;
};

class PointLight : public Light {
public:
    PointLight(const std::string &name, const Transform &transform, const Color3 &color, float range);

    void setParameters(int i, Shader *shader) const override;

    const Mat4 &projection() const override;

    ALIT_API float range() const;

    ALIT_API void setRange(float range);

private:
    static const float maxRange;
    static const Mat4 m_projection;

    float m_range;
};

class DirectionalLight : public Light {
public:
    DirectionalLight(const std::string &name, const Transform &transform, const Color3 &color);

    void setParameters(int i, Shader *shader) const override;

    const Mat4 &projection() const override;

private:
    static const Mat4 m_projection;
};

class SpotLight : public Light {
public:
    SpotLight(const std::string &name, const Transform &transform, const Color3 &color, float range, float innerAngle,
              float outerAngle);

    void setParameters(int i, Shader *shader) const override;

    const Mat4 &projection() const override;

    ALIT_API float range() const;

    ALIT_API void setRange(float range);

    ALIT_API float innerAngle() const;

    ALIT_API void setInnerAngle(float angle);

    ALIT_API float outerAngle() const;

    ALIT_API void setOuterAngle(float angle);

private:
    static const float maxRange;
    static const Mat4 m_projection;

    float m_range;
    float m_cosInnerAngle;
    float m_cosOuterAngle;
};

class AmbientLight : public Light {
public:
    AmbientLight(const std::string &name, const Transform &transform, const Color3 &color);

    void setParameters(int i, Shader *shader) const override;

    const Mat4 &projection() const override;

private:
    static const Mat4 m_projection;
};

} // namespace ALIT

#include <Core/Base/inline/Light.inl>

#endif