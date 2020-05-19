#pragma once

#include "glad.h"

#include <vector>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>


namespace AVLIT {

class OGLVAO {
public:
    OGLVAO(const OGLVAO &) = delete;
    void operator=(const OGLVAO &) = delete;

    OGLVAO() = default;

    OGLVAO(const Mesh &mesh);

    ~OGLVAO();

    inline void operator=(OGLVAO &&vao) noexcept;

    inline bool operator==(const OGLVAO &vao) const;

    inline void bind() const;

    static inline void unbindAll();

private:
    GLuint m_vaoID{0};
    std::vector<GLuint> m_buffers;
};

} // namespace AVLIT

#include <Core/Renderer/OpenGL/inline/OGLVAO.inl>