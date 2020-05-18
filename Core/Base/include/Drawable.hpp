#pragma once

#include <Core/Base/include/SceneObject.hpp>


namespace AVLIT {

class Drawable : public SceneObject {
public:
    Drawable(const std::string &name, const Transform &transform, Model *model);
    virtual ~Drawable() = default;

    AVLIT_API void scale(const Mat3 &scaling) override;

    const Model *model() const override;

private:
    void _scale(const Mat3 &scaling);

    Model *m_model;
};

} // namespace AVLIT