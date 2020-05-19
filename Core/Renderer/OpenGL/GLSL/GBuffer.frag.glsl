#version 410 core

layout(location = 0) in vec4 texCoordViewZNormalX;
layout(location = 1) in vec4 tangentNormalY;
layout(location = 2) in vec4 bitangentNormalZ;

layout(location = 0) out vec4 outNormalZ;
layout(location = 1) out vec4 outAlbedo;
layout(location = 2) out vec2 outMetalnessRoughness;


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
    vec2 textureCoord = texCoordViewZNormalX.xy;

    float alpha = material.hasAlphaMap ? texture(material.alphaMap, textureCoord).x : material.alpha;
    if(alpha < 0.25)
        discard;

    float viewZ = texCoordViewZNormalX.z;

    // Unpacking everything
    vec3 normal = vec3(texCoordViewZNormalX.w, tangentNormalY.w, bitangentNormalZ.w);
    vec3 tangent = tangentNormalY.xyz;
    vec3 bitangent = bitangentNormalZ.xyz;

    outNormalZ = vec4(material.hasNormalMap ? normalize(mat3(tangent, bitangent, normal) *
                                                        (2.f * texture(material.normalMap, textureCoord).xyz - 1.f))
                                            : normal,
                      viewZ);

    outAlbedo = vec4(material.hasAlbedoMap ? texture(material.albedoMap, textureCoord).xyz : material.albedo, alpha);

    outMetalnessRoughness = vec2(material.hasMetalnessMap ? texture(material.metalnessMap, textureCoord).x : 0.25f,
                                 material.hasRoughnessMap ? texture(material.roughnessMap, textureCoord).x : 1.f);
}