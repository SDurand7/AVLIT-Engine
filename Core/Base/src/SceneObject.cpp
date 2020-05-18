#include "SceneObject.hpp"

#include <Core/Base/include/Math.hpp>


namespace AVLIT {

SceneObject::SceneObject(const std::string &name, const Transform &transform)
    : m_name{name}, m_transform{transform}, m_aabb{transform[3]}, m_visible{true} {}

void SceneObject::scale(const Mat3 &trf) {
    if(m_graphNode)
        m_graphNode->scale(trf);
}

SceneBVHNode *SceneObject::graphNode() const { return m_graphNode; }

void SceneObject::setGraphNode(SceneBVHNode *graphNode) { m_graphNode = graphNode; }

void SceneObject::setName(const std::string &name) { m_name = name; }

const Model *SceneObject::model() const { return nullptr; }

} // namespace AVLIT