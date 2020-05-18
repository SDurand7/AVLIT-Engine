#include "MaterialManager.hpp"


namespace AVLIT {

Material *MaterialManager::material(const std::string &name) const {
    auto matIt = m_materials.find(name);
    if(matIt == m_materials.cend())
        return nullptr;
    return matIt->second.get();
}

void MaterialManager::addMaterial(const std::string &name, MaterialUptr &&material) {
    m_materials.emplace(name, std::move(material));
}

} // namespace AVLIT