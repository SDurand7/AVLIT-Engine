#include <Core/Renderer/include/Shader.hpp>
#include <Core/Renderer/include/Texture.hpp>


namespace AVLIT {

// TODO: handle the case where we have an alpha map
inline bool Material::isOpaque() const { return m_alpha > 0.99; }

inline bool Material::isTwoSided() const { return m_twoSided; }

inline Color3 Material::albedo() const { return m_albedo; }

inline float Material::alpha() const { return m_alpha; }

inline void Material::setParameters(const std::string &name, Shader *shader) const {
    shader->setUniform(name + ".hasNormalMap", static_cast<bool>(m_normalMap));
    shader->setUniform(name + ".hasAlbedoMap", static_cast<bool>(m_albedoMap));
    shader->setUniform(name + ".hasMetalnessMap", static_cast<bool>(m_metalnessMap));
    shader->setUniform(name + ".hasRoughnessMap", static_cast<bool>(m_roughnessMap));
    shader->setUniform(name + ".hasAlphaMap", static_cast<bool>(m_alphaMap));

    if(m_normalMap) {
        m_normalMap->bind(0);
        shader->setUniform(name + ".normalMap", 0);
    }
    if(m_albedoMap) {
        m_albedoMap->bind(1);
        shader->setUniform(name + ".albedoMap", 1);
    }
    if(m_metalnessMap) {
        m_metalnessMap->bind(2);
        shader->setUniform(name + ".metalnessMap", 2);
    }
    if(m_roughnessMap) {
        m_roughnessMap->bind(3);
        shader->setUniform(name + ".roughnessMap", 3);
    }
    if(m_alphaMap) {
        m_alphaMap->bind(4);
        shader->setUniform(name + ".alphaMap", 4);
    }

    shader->setUniform(name + ".albedo", m_albedo);
    shader->setUniform(name + ".alpha", m_alpha);
}

inline const Texture *Material::normalMap() const { return m_normalMap; }

inline const Texture *Material::albedoMap() const { return m_albedoMap; }

inline const Texture *Material::metalnessMap() const { return m_metalnessMap; }

inline const Texture *Material::roughnessMap() const { return m_roughnessMap; }

inline const Texture *Material::alphaMap() const { return m_alphaMap; }

} // namespace AVLIT