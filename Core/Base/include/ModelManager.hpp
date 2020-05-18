#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Base/include/Model.hpp>


namespace AVLIT {

class ModelManager {
public:
    ModelManager() = default;

    const std::vector<std::string> &names() const;

    AVLIT_API Model *model(const std::string &name) const;

    void addModel(const std::string &name, ModelUptr &&model);

private:
    std::vector<std::string> m_names;

    std::unordered_map<std::string, ModelUptr> m_models;
};

} // namespace AVLIT