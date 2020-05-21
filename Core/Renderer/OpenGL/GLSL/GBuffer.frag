#version 410 core

layout(location = 0) out vec4 outNormalZ;
layout(location = 1) out vec4 outAlbedo;
layout(location = 2) out vec2 outMetalnessRoughness;

in Data {
    float viewZ;
    vec2 textureCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
} fs_in;


struct Material {
    bool hasNormalMap;
    bool hasAlbedoMap;
    bool hasMetalnessMap;
    bool hasRoughnessMap;
    bool hasAlphaMap;

    sampler2D normalMap;
    sampler2D albedoMap;
    sampler2D metalnessMap;
    sampler2D roughnessMap;
    sampler2D alphaMap;

    vec3 albedo;
    float alpha;
};


uniform Material material;


void main() {
    vec2 textureCoord = fs_in.textureCoord;

    float alpha = material.hasAlphaMap ? texture(material.alphaMap, textureCoord).x : material.alpha;
    if(alpha < 0.25f)
        discard;

    // Re-normalize everything after interpolation
    vec3 normal = normalize(fs_in.normal);
    vec3 tangent = normalize(fs_in.tangent);
    vec3 bitangent = normalize(fs_in.bitangent);

    outNormalZ = vec4(material.hasNormalMap ? normalize(mat3(tangent, bitangent, normal) *
                                                        (2.f * texture(material.normalMap, textureCoord).xyz - 1.f))
                                            : normal,
                      fs_in.viewZ);

    outAlbedo = vec4(material.hasAlbedoMap ? texture(material.albedoMap, textureCoord).xyz : material.albedo, alpha);

    outMetalnessRoughness = vec2(material.hasMetalnessMap ? texture(material.metalnessMap, textureCoord).x : 0.25f,
                                 material.hasRoughnessMap ? texture(material.roughnessMap, textureCoord).x : 1.f);
}