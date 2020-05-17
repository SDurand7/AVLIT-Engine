#ifndef ALIT_MATH_HPP
#define ALIT_MATH_HPP

#include <cmath>

#include <Dependencies/glm/glm/gtx/transform.hpp>
#include <Core/Base/include/Types.hpp>


namespace ALIT {

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
using std::cos;
using std::sin;
using std::sqrt;
using std::atan2;

inline Mat4 rotate(const Vec3 &axis, float angle) { return glm::rotate(angle, axis); }

inline float pi() { return glm::pi<float>(); }

} // namespace ALIT

#endif