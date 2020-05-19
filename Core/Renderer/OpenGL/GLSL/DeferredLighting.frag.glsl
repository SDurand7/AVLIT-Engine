#version 410 core

#include "Light.glsl"

layout(location = 0) in vec2 textureCoord;

layout(location = 0) out vec4 outFragColor;

uniform sampler2D normalZMap;
uniform sampler2D albedoMap; // + alpha
uniform sampler2D metalnessRoughnessMap;

uniform sampler2D occlusionMap;

uniform sampler2DShadow shadowMap;

uniform samplerCube skybox;

uniform mat4 lightVP;

uniform mat4 inverseView;
uniform mat4 inverseProjection;

uniform Light light;


float shadow(vec4 position, vec3 normal, vec3 lightV) {
    position.xyz += normal * 0.125f;
    position = lightVP * position;
    position /= position.w;
    position = position * 0.5f + 0.5f;
    vec2 texelSize = 1.f / textureSize(shadowMap, 0);

    float result = 0.f;
    // TODO: iterate using ints
    for(float i = -1.5f; i <= 1.5f + 0.001f; i += 1.f) {
        for(float j = -1.5f; j <= 1.5f + 0.001f; j += 1.f) {
            result += texture(shadowMap, vec3(position.xy + vec2(i, j) * texelSize, position.z));
        }
    }

    return result / 16;
}


void main() {
    vec4 albedo = texture(albedoMap, textureCoord);

    if(light.type == AMBIENT_LIGHT) {
        outFragColor = vec4(texture(occlusionMap, textureCoord).x * light.color * albedo.rgb, albedo.a);
    } else {
        vec4 normalZ = texture(normalZMap, textureCoord);
        vec3 normal = normalZ.xyz;
        float viewZ = normalZ.w;

        vec2 metalnessRoughness = texture(metalnessRoughnessMap, textureCoord).xy;
        float metalness = metalnessRoughness.x;
        float roughness = metalnessRoughness.y;

        // Re-compute position from viewZ
        vec3 position = vec3(0.f);

        vec3 wi = normalize(light.position - position);
        vec3 wo = normalize(inverseView[3].xyz - position);

        // Shade
    }
}