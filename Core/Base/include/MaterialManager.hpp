#ifndef ALIT_MATERIALMANAGER_HPP
#define ALIT_MATERIALMANAGER_HPP

#include <unordered_map>
#include <string>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Base/include/Material.hpp>


namespace ALIT {

class MaterialManager {
    // TODO: check if two material with the same name are actually identical (especially the unamed ones)
public:
    MaterialManager() = default;

    Material *material(const std::string &name) const;

    void addMaterial(const std::string &name, MaterialUptr &&material);

private:
    std::unordered_map<std::string, MaterialUptr> m_materials;
};

} // namespace ALIT

#endif