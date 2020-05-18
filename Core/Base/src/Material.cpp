#include "Material.hpp"


namespace AVLIT {

void Material::setKa(Color3 ka) { m_ka = ka; }
void Material::setKd(Color3 kd) { m_kd = kd; }
void Material::setKs(Color3 ks) { m_ks = ks; }
void Material::setNs(float ns) { m_ns = ns; }
void Material::setNsStrength(float nsStrength) { m_nsStrength = nsStrength; }
void Material::setAlpha(float alpha) { m_alpha = alpha; }

void Material::setNormalTexture(const Texture *normalTexture) { m_normalTex = normalTexture; }
void Material::setKaTexture(const Texture *kaTexture) { m_kaTex = kaTexture; }
void Material::setKdTexture(const Texture *kdTexture) { m_kdTex = kdTexture; }
void Material::setKsTexture(const Texture *ksTexture) { m_ksTex = ksTexture; }
void Material::setAlphaTexture(const Texture *alphaTexture) { m_alphaTex = alphaTexture; }

void Material::setTwoSided(bool twoSided) { m_twoSided = twoSided; }

} // namespace AVLIT