#include "ModelManager.hpp"


namespace AVLIT {

const std::vector<std::string> &ModelManager::names() const { return m_names; }

Model *ModelManager::model(const std::string &name) const {
    auto modelIt = m_models.find(name);
    if(modelIt == m_models.cend())
        return nullptr;
    return modelIt->second.get();
}

void ModelManager::addModel(const std::string &name, ModelUptr &&model) { m_models.emplace(name, std::move(model)); }

} // namespace AVLIT