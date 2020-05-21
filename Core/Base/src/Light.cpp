#include "Light.hpp"

#include <Core/Base/include/Math.hpp>
#include <Core/Base/include/Model.hpp>
#include <Core/Base/include/Mesh.hpp>


namespace AVLIT {

const float nearZ = 0.85f;

/// Light base class
Light::Light(const std::string &name, const Transform &transform, const Color3 &color)
    : SceneObject(name, transform), m_color(color) {
    m_transform[0] = normalize(m_transform[0]);
    m_transform[1] = normalize(m_transform[1]);
    m_transform[2] = normalize(m_transform[2]);
}


/// PointLight derived class
const float PointLight::maxRange = 100.f;
const Mat4 PointLight::m_projection = perspective(pi() / 2, 1.f, nearZ, maxRange);

PointLight::PointLight(const std::string &name, const Transform &transform, const Color3 &color)
    : Light{name, transform, color} {}

void PointLight::setParameters(const std::string &name, Shader *shader) const {
    shader->setUniform(name + ".type", (uint)LightType::POINT_LIGHT);
    shader->setUniform(name + ".color", m_color);
    shader->setUniform(name + ".position", m_transform[3]);
}

const Mat4 &PointLight::projection() const { return m_projection; }


/// DirectionalLight derived class
const Mat4 DirectionalLight::m_projection = ortho(-75.f, 75.f, -80.f, 90.f, -80.f, 150.f);

DirectionalLight::DirectionalLight(const std::string &name, const Transform &transform, const Color3 &color)
    : Light{name, transform, color} {}

void DirectionalLight::setParameters(const std::string &name, Shader *shader) const {
    shader->setUniform(name + ".type", static_cast<uint>(LightType::DIRECTIONAL_LIGHT));
    shader->setUniform(name + ".color", m_color);
    shader->setUniform(name + ".direction", -m_transform[2]);
}

const Mat4 &DirectionalLight::projection() const { return m_projection; }


/// SpotLight derived class
const float SpotLight::maxRange = 100.f;
const Mat4 SpotLight::m_projection = perspective(0.75f * pi(), 1.f, nearZ, maxRange);

SpotLight::SpotLight(const std::string &name, const Transform &transform, const Color3 &color, float innerAngle,
                     float outerAngle)
    : Light{name, transform, color}, m_cosInnerAngle{cos(innerAngle)}, m_cosOuterAngle{cos(outerAngle)} {}

void SpotLight::setParameters(const std::string &name, Shader *shader) const {
    shader->setUniform(name + ".type", static_cast<uint>(LightType::SPOT_LIGHT));
    shader->setUniform(name + ".color", m_color);
    shader->setUniform(name + ".position", m_transform[3]);
    shader->setUniform(name + ".direction", -m_transform[2]);
    shader->setUniform(name + ".cosInnerAngle", m_cosInnerAngle);
    shader->setUniform(name + ".cosOuterAngle", m_cosOuterAngle);
}

const Mat4 &SpotLight::projection() const { return m_projection; }

float SpotLight::innerAngle() const { return acos(m_cosInnerAngle); }

void SpotLight::setInnerAngle(float angle) { m_cosInnerAngle = cos(angle); }

float SpotLight::outerAngle() const { return acos(m_cosOuterAngle); }

void SpotLight::setOuterAngle(float angle) { m_cosOuterAngle = cos(angle); }


/// AmbientLight derived class
const Mat4 AmbientLight::m_projection = Mat4{};

AmbientLight::AmbientLight(const std::string &name, const Transform &transform, const Color3 &color)
    : Light{name, transform, color} {}

void AmbientLight::setParameters(const std::string &name, Shader *shader) const {
    shader->setUniform(name + ".type", static_cast<uint>(LightType::AMBIENT_LIGHT));
    shader->setUniform(name + ".color", m_color);
}

const Mat4 &AmbientLight::projection() const { return m_projection; }

} // namespace AVLIT