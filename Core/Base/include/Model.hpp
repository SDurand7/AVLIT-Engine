#ifndef ALIT_MODEL_HPP
#define ALIT_MODEL_HPP

#include <vector>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Base/include/AABB.hpp>


namespace ALIT {

class Model {
public:
    Model() = default;

    void addMesh(Mesh *mesh);

    const AABB &aabb() const;

    ALIT_API const std::vector<std::pair<Material *, std::vector<Mesh *>>> &meshesByMaterial() const;

private:
    std::vector<std::pair<Material *, std::vector<Mesh *>>> m_meshesByMaterial;

    AABB m_aabb;
};

} // namespace ALIT

#endif