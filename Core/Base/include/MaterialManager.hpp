#pragma once

#include <unordered_map>
#include <string>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Base/include/Material.hpp>


namespace AVLIT {

class MaterialManager {
public:
    MaterialManager() = default;

    Material *material(const std::string &name) const;

    void addMaterial(const std::string &name, MaterialUptr &&material);

private:
    std::unordered_map<std::string, MaterialUptr> m_materials;
};

} // namespace AVLIT