#include <Core/Renderer/include/Shader.hpp>
#include <Core/Renderer/include/Texture.hpp>


namespace AVLIT {

// TODO: handle the case where we have an alpha map
inline bool Material::isOpaque() const { return m_alpha > 0.99; }

inline bool Material::isTwoSided() const { return m_twoSided; }

inline void Material::setParameters(const std::string &name, Shader *shader) const {
    shader->setUniform(name + ".hasNormalTexture", static_cast<bool>(m_normalTex));
    shader->setUniform(name + ".hasKaTexture", static_cast<bool>(m_kaTex));
    shader->setUniform(name + ".hasKdTexture", static_cast<bool>(m_kdTex));
    shader->setUniform(name + ".hasKsTexture", static_cast<bool>(m_ksTex));
    shader->setUniform(name + ".hasAlphaTexture", static_cast<bool>(m_alphaTex));

    if(m_normalTex)
        m_normalTex->bind(name + ".normalTexture", shader, 0);
    if(m_kaTex)
        m_kaTex->bind(name + ".kaTexture", shader, 1);
    if(m_kdTex)
        m_kdTex->bind(name + ".kdTexture", shader, 2);
    if(m_ksTex)
        m_ksTex->bind(name + ".ksTexture", shader, 3);
    if(m_alphaTex)
        m_alphaTex->bind(name + ".alphaTexture", shader, 4);

    shader->setUniform(name + ".ka", m_ka);
    shader->setUniform(name + ".kd", m_kd);
    shader->setUniform(name + ".ks", m_ks);
    shader->setUniform(name + ".ns", m_ns);
    shader->setUniform(name + ".nsStrength", m_nsStrength);
    shader->setUniform(name + ".alpha", m_alpha);
}

} // namespace AVLIT