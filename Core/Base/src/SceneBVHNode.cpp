#include "SceneBVHNode.hpp"


namespace ALIT {

SceneBVHNode::SceneBVHNode(SceneObject *object) : m_children{}, m_sceneObject{object} {
    if(m_sceneObject)
        m_aabb = m_sceneObject->aabb();
}

SceneBVHNode::SceneBVHNode(SceneBVHNode &&node)
    : m_children{std::move(node.m_children)}, m_parent{node.m_parent},
      m_sceneObject{node.m_sceneObject}, m_aabb{std::move(node.m_aabb)} {}

void SceneBVHNode::addChild(SceneBVHNode *child) {
    if(!m_sceneObject && m_children.empty())
        m_aabb = child->aabb();
    else
        m_aabb.merge(child->aabb());

    auto parent = m_parent;
    const auto *aabb = &m_aabb;
    while(parent) {
        parent->m_aabb.merge(*aabb);
        aabb = &parent->m_aabb;
        parent = parent->m_parent;
    }

    child->m_parent = this;

    m_children.emplace_back(child);
}

SceneBVHNode *SceneBVHNode::parent() const { return m_parent; }

void SceneBVHNode::removeChild(SceneBVHNode *child) {
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());

    updateAABB();

    updateParentsAABBs();
}

void SceneBVHNode::transferTo(SceneBVHNode *newParent) {
    m_parent->removeChild(this);

    newParent->addChild(this);
}

SceneBVHNode *SceneBVHNode::child(int i) const { return m_children[i]; }

void SceneBVHNode::updateAABB() {
    if(m_children.empty()) {
        if(!m_sceneObject) {
            m_aabb = AABB{Vec3{0.f}};
        } else {
            m_aabb = m_sceneObject->aabb();
        }
    } else {
        int i = 0;
        if(!m_sceneObject) {
            m_aabb = m_children[0]->aabb();
            i = 1;
        } else {
            m_aabb = m_sceneObject->aabb();
        }

        for(; i < static_cast<int>(m_children.size()); ++i) {
            m_aabb.merge(m_children[i]->aabb());
        }
    }
}

void SceneBVHNode::updateParentsAABBs() {
    auto current = m_parent;

    while(current) {
        current->updateAABB();
        current = current->m_parent;
    }
}

} // namespace ALIT