#include "Drawable.hpp"

#include <Core/Base/include/Model.hpp>
#include <Core/Base/include/Mesh.hpp>


namespace AVLIT {

Drawable::Drawable(const std::string &name, const Transform &transform, Model *model)
    : SceneObject{name, transform}, m_model{model} {
    m_aabb.compute(m_model, m_transform);
}

void Drawable::scale(const Mat3 &scaling) {
    _scale(scaling);
    m_graphNode->updateParentsAABBs();
}

const Model *Drawable::model() const { return m_model; }

void Drawable::_scale(const Mat3 &scaling) {
    m_transform = m_transform * Mat4{scaling};
    m_aabb.compute(m_model, m_transform);
    SceneObject::scale(scaling);
}

} // namespace AVLIT