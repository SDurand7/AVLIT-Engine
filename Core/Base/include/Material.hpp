#pragma once

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Renderer/include/Shader.hpp>
#include <Core/Renderer/include/Texture.hpp>


namespace AVLIT {

class Material {
public:
    Material() = default;

    inline bool isOpaque() const;

    inline bool isTwoSided() const;

    inline Color3 albedo() const;

    inline float alpha() const;

    inline void setParameters(const std::string &name, Shader *shader) const;

    void setAlbedo(Color3 albedo);

    void setAlpha(float alpha);

    void setNormalMap(const Texture *normalMap);

    void setAlbedoMap(const Texture *albedoMap);

    void setMetalnessMap(const Texture *metalnessMap);

    void setRoughnessMap(const Texture *roughnessMap);

    void setAlphaMap(const Texture *alphaMap);

    void setTwoSided(bool twoSided);

    inline const Texture *normalMap() const;

    inline const Texture *albedoMap() const;

    inline const Texture *metalnessMap() const;

    inline const Texture *roughnessMap() const;

    inline const Texture *alphaMap() const;

private:
    Color3 m_albedo = {1.f, 1.f, 1.f};
    float m_alpha = 1.f;

    const Texture *m_normalMap = nullptr;
    const Texture *m_albedoMap = nullptr;
    const Texture *m_metalnessMap = nullptr;
    const Texture *m_roughnessMap = nullptr;
    const Texture *m_alphaMap = nullptr;

    bool m_twoSided = false;
};

} // namespace AVLIT

#include <Core/Base/inline/Material.inl>