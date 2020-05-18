#pragma once

#include <cmath>

#include <Dependencies/glm/glm/gtx/transform.hpp>
#include <Core/Base/include/Types.hpp>


namespace AVLIT {

using glm::cross;
using glm::dot;
using glm::epsilon;
using glm::inverse;
using glm::length;
using glm::lookAt;
using glm::normalize;
using glm::ortho;
using glm::perspective;
using glm::transpose;
using std::acos;
using std::asin;
using std::atan2;
using std::cos;
using std::sin;
using std::sqrt;

inline Mat4 rotate(const Vec3 &axis, float angle) { return glm::rotate(angle, axis); }

inline float pi() { return glm::pi<float>(); }

} // namespace AVLIT