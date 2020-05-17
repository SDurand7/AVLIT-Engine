#include <Core/Renderer/include/Shader.hpp>
#include <Core/Renderer/include/Texture.hpp>


namespace ALIT {

// TODO: handle the case where we have an alpha map
inline bool Material::isOpaque() const { return m_alpha > 0.99; }

inline bool Material::isTwoSided() const { return m_twoSided; }

inline void Material::setParameters(int i, Shader *shader) const {
    shader->setUniform(i, m_normalTex);
    shader->setUniform(i + 1, m_kaTex);
    shader->setUniform(i + 2, m_kdTex);
    shader->setUniform(i + 3, m_ksTex);
    shader->setUniform(i + 4, m_alphaTex);

    if(m_normalTex)
        m_normalTex->bind(i + 5, shader, 0);
    if(m_kaTex)
        m_kaTex->bind(i + 6, shader, 1);
    if(m_kdTex)
        m_kdTex->bind(i + 7, shader, 2);
    if(m_ksTex)
        m_ksTex->bind(i + 8, shader, 3);
    if(m_alphaTex)
        m_alphaTex->bind(i + 9, shader, 4);

    shader->setUniform(i + 10, m_ka);
    shader->setUniform(i + 11, m_kd);
    shader->setUniform(i + 12, m_ks);
    shader->setUniform(i + 13, m_ns);
    shader->setUniform(i + 14, m_nsStrength);
    shader->setUniform(i + 15, m_alpha);
}

} // namespace ALIT