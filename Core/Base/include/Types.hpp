#ifndef ALIT_TYPES_HPP
#define ALIT_TYPES_HPP

#include <memory>

#include <Dependencies/glm/glm/glm.hpp>
#include <Dependencies/glm/glm/gtc/type_ptr.hpp>


namespace ALIT {

using uint = unsigned int;
using ulong = unsigned long;
using ushort = unsigned short;
using uchar = unsigned char;

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;
using Transform = glm::mat4x3;

using Color3 = Vec3;
using Color4 = Vec4;

using glm::value_ptr;

#ifdef DX_
/// Coming soon                                                                                            ... maybe not
#else
class OGLRenderSystem;
class OGLShader;
class OGLVAO;
class OGLTexture;

using RenderSystem = OGLRenderSystem;
using Shader = OGLShader;
using GPUBuffer = OGLVAO;
using Texture = OGLTexture;
#endif

using RenderSystemUptr = std::unique_ptr<RenderSystem>;
using TextureUptr = std::unique_ptr<Texture>;

enum class LightType { POINT_LIGHT = 0, DIRECTIONAL_LIGHT = 1, SPOT_LIGHT = 2, AMBIENT_LIGHT = 3 };

class AssetManager;
class Drawable;
class Light;
class Camera;
class Mesh;
class MeshManager;
class Model;
class ModelManager;
class Material;
class MaterialManager;
class TextureManager;
class Scene;
class SceneBVHNode;
class SceneObject;
class AABB;
class Ray;

using AssetManagerUptr = std::unique_ptr<AssetManager>;
using DrawableUptr = std::unique_ptr<Drawable>;
using LightUptr = std::unique_ptr<Light>;
using CameraUptr = std::unique_ptr<Camera>;
using MeshUptr = std::unique_ptr<Mesh>;
using MeshManagerUptr = std::unique_ptr<MeshManager>;
using ModelUptr = std::unique_ptr<Model>;
using ModelManagerUptr = std::unique_ptr<ModelManager>;
using MaterialUptr = std::unique_ptr<Material>;
using MaterialManagerUptr = std::unique_ptr<MaterialManager>;
using TextureManagerUptr = std::unique_ptr<TextureManager>;
using SceneUptr = std::unique_ptr<Scene>;
using SceneBVHNodeUptr = std::unique_ptr<SceneBVHNode>;
using SceneObjectUptr = std::unique_ptr<SceneObject>;

} // namespace ALIT

#endif