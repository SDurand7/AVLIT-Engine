#include "Model.hpp"

#include <algorithm>

#include <Core/Base/include/Mesh.hpp>
#include <Core/Base/include/Material.hpp>


namespace ALIT {

void Model::addMesh(Mesh *mesh) {
    auto material = mesh->material();

    auto res = std::find_if(
        m_meshesByMaterial.begin(), m_meshesByMaterial.end(),
        [&material](const std::pair<Material *, std::vector<Mesh *>> &pair) { return pair.first == material; });

    if(res == m_meshesByMaterial.end())
        m_meshesByMaterial.emplace_back(material, std::vector<Mesh *>{mesh});
    else
        res->second.emplace_back(mesh);
    m_aabb.merge(mesh->aabb());
}

const AABB &Model::aabb() const { return m_aabb; }

const std::vector<std::pair<Material *, std::vector<Mesh *>>> &Model::meshesByMaterial() const {
    return m_meshesByMaterial;
}

} // namespace ALIT