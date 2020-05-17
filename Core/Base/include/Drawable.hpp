#ifndef ALIT_DRAWABLE_HPP
#define ALIT_DRAWABLE_HPP

#include <Core/Base/include/SceneObject.hpp>


namespace ALIT {

class Drawable : public SceneObject {
public:
    Drawable(const std::string &name, const Transform &transform, Model *model);
    virtual ~Drawable() = default;

    ALIT_API void scale(const Mat3 &scaling) override;

    const Model *model() const override;

private:
    void _scale(const Mat3 &scaling);

    Model *m_model;
};

} // namespace ALIT

#endif