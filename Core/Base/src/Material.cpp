#include "Material.hpp"


namespace AVLIT {

void Material::setAlbedo(Color3 albedo) { m_albedo = albedo; }

void Material::setAlpha(float alpha) { m_alpha = alpha; }

void Material::setNormalMap(const Texture *normalMap) { m_normalMap = normalMap; }

void Material::setAlbedoMap(const Texture *albedoMap) { m_albedoMap = albedoMap; }

void Material::setMetalnessMap(const Texture *metalnessMap) { m_metalnessMap = metalnessMap; }

void Material::setRoughnessMap(const Texture *roughnessMap) { m_roughnessMap = roughnessMap; }

void Material::setAlphaMap(const Texture *alphaMap) { m_alphaMap = alphaMap; }

void Material::setTwoSided(bool twoSided) { m_twoSided = twoSided; }

} // namespace AVLIT