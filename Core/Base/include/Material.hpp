#pragma once

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>


namespace AVLIT {

class Material {
public:
    Material() = default;

    inline bool isOpaque() const;

    inline bool isTwoSided() const;

    inline void setParameters(int i, Shader *shader) const;

    // TODO: replace all those setters by a setter for each type and a flag (?)
    void setKa(Color3 ka);
    void setKd(Color3 kd);
    void setKs(Color3 ks);
    void setNs(float ns);
    void setNsStrength(float nsStrength);
    void setAlpha(float alpha);

    void setNormalTexture(const Texture *normalTexture);
    void setKaTexture(const Texture *kaTexture);
    void setKdTexture(const Texture *kdTexture);
    void setKsTexture(const Texture *ksTexture);
    void setAlphaTexture(const Texture *alphaTexture);

    void setTwoSided(bool twoSided);

private:
    Color3 m_ka = {1.f, 1.f, 1.f};
    Color3 m_kd = {1.f, 1.f, 1.f};
    Color3 m_ks = {1.f, 1.f, 1.f};
    float m_ns = 1.f;
    float m_nsStrength = 0.25f;
    float m_alpha = 1.f;

    const Texture *m_normalTex = nullptr;
    const Texture *m_kaTex = nullptr;
    const Texture *m_kdTex = nullptr;
    const Texture *m_ksTex = nullptr;
    const Texture *m_alphaTex = nullptr;

    bool m_twoSided = false;
};

} // namespace AVLIT

#include <Core/Base/inline/Material.inl>