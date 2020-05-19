#version 410 core

layout(location = 0) in vec2 textureCoord;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in float viewZ;

layout(location = 0) out vec4 outNormalZ;
layout(location = 1) out vec4 outAmbientColor;
layout(location = 2) out vec3 outDiffuseColor;
layout(location = 3) out vec3 outSpecularColor;
layout(location = 4) out vec2 outSpecularParameters;


struct Material {
    // boolean for vertex colors ?
    bool hasNormalTexture;
    bool hasKaTexture;
    bool hasKdTexture;
    bool hasKsTexture;
    bool hasAlphaTexture;

    sampler2D normalTexture;
    sampler2D kaTexture;
    sampler2D kdTexture;
    sampler2D ksTexture;
    sampler2D alphaTexture;

    vec3 ka;
    vec3 kd;
    vec3 ks;
    float ns;
    float nsStrength;
    float alpha;
};


uniform Material material;


void main() {
    float alpha = material.hasAlphaTexture ? texture(material.alphaTexture, textureCoord).x : material.alpha;

    if(alpha < 0.25)
        discard;

    outNormalZ = vec4(material.hasNormalTexture ? mat3(tangent, bitangent, normal) *
                                                      (2.0 * texture(material.normalTexture, textureCoord).xyz - 1.0)
                                                : normal,
                      viewZ);
    outAmbientColor = vec4(material.hasKaTexture
                               ? texture(material.kaTexture, textureCoord).xyz
                               : (material.hasKdTexture ? texture(material.kdTexture, textureCoord).xyz : material.ka),
                           alpha);
    outDiffuseColor = material.hasKdTexture ? texture(material.kdTexture, textureCoord).xyz : material.kd;
    outSpecularColor = material.hasKsTexture ? texture(material.ksTexture, textureCoord).xyz : material.ks;
    outSpecularParameters = vec2(material.ns, material.nsStrength);
}