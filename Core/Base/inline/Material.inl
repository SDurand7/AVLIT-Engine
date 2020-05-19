#include <Core/Renderer/include/Shader.hpp>
#include <Core/Renderer/include/Texture.hpp>


namespace AVLIT {

// TODO: handle the case where we have an alpha map
inline bool Material::isOpaque() const { return m_alpha > 0.99; }

inline bool Material::isTwoSided() const { return m_twoSided; }

inline void Material::setParameters(const std::string &name, Shader *shader) const {
    shader->setUniform(name + ".hasNormalMap", static_cast<bool>(m_normalMap));
    shader->setUniform(name + ".hasAlbedoMap", static_cast<bool>(m_albedoMap));
    shader->setUniform(name + ".hasMetalnessMap", static_cast<bool>(m_metalnessMap));
    shader->setUniform(name + ".hasRoughnessMap", static_cast<bool>(m_roughnessMap));
    shader->setUniform(name + ".hasAlphaMap", static_cast<bool>(m_alphaMap));

    if(m_normalMap)
        m_normalMap->bind(name + ".normalMap", shader, 0);
    if(m_albedoMap)
        m_albedoMap->bind(name + ".albedoMap", shader, 1);
    if(m_metalnessMap)
        m_metalnessMap->bind(name + ".metalnessMap", shader, 2);
    if(m_roughnessMap)
        m_roughnessMap->bind(name + ".roughnessMap", shader, 3);
    if(m_alphaMap)
        m_alphaMap->bind(name + ".alphaMap", shader, 4);

    shader->setUniform(name + ".albedo", m_albedo);
    shader->setUniform(name + ".alpha", m_alpha);
}

} // namespace AVLIT