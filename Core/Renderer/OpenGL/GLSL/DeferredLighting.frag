#version 460 core

#define PI 3.14159265359
#define LIGHT_BATCH_SIZE 5

#include "Light.glsl"

layout(location = 0) in vec2 textureCoord;

layout(location = 0) out vec4 outFragColor;

uniform sampler2D normalZMap;
uniform sampler2D albedoMap; // + alpha
uniform sampler2D metalnessRoughnessMap;

uniform sampler2D occlusionMap;

uniform mat4 inverseView;
uniform mat4 inverseProjection;

uniform Light lights[LIGHT_BATCH_SIZE];

uniform uint batchSize;


float shadow(Light light, vec4 position, vec3 normal) {
    position.xyz += normal * 0.05f;
    position = light.lightVP * position;
    position /= position.w;
    position = position * 0.5f + 0.5f;
    vec2 texelSize = 1.f / textureSize(light.shadowMap, 0);

    float result = 0.f;
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            result += texture(light.shadowMap, vec3(position.xy + vec2(i - 1.5f, j - 1.5f) * texelSize, position.z - 0.00065f));
        }
    }

    return result / 16;
}

float occlusion() {
    vec2 texelSize = 1.f / textureSize(occlusionMap, 0);
    float result = 0.f;

    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            result += texture(occlusionMap, textureCoord + texelSize * vec2(i - 1.5f, j - 1.5f)).x;
        }
    }

    return result / 16;
}

// Schlick's approximation
vec3 fresnel(float hvDotWo, vec3 f0) { return f0 + (1.f - f0) * pow(1.f - hvDotWo, 5.f); }

float distributionGGX(vec3 normal, vec3 hv, float roughness) {
    float roughness2 = roughness * roughness;
    float roughness4 = roughness2 * roughness2;

    float ndoth = max(dot(normal, hv), 0.f);

    float div = (ndoth * ndoth * (roughness4 - 1.f) + 1.f);
    div = PI * div * div;

    return roughness4 / div;
}

// Schlick, again
float geometryGGX(float cosTheta, float roughness) {
    float r = roughness + 1.f;
    float k = r * r / 8.f;

    float div = cosTheta * (1.f - k) + k;

    return cosTheta / div;
}

float geometrySmith(float cosThetao, float cosThetai, float roughness) {
    return geometryGGX(cosThetao, roughness) * geometryGGX(cosThetai, roughness);
}


void main() {
    vec4 albedo = texture(albedoMap, textureCoord);
    vec4 normalZ = texture(normalZMap, textureCoord);
    vec3 normal = normalZ.xyz;
    float viewZ = normalZ.w;

    vec2 metalnessRoughness = texture(metalnessRoughnessMap, textureCoord).xy;
    float metalness = metalnessRoughness.x;
    float roughness = metalnessRoughness.y;

    // Re-compute position from viewZ
    vec4 direction = vec4(2.f * textureCoord - 1.f, 0.f, 1.f);
    direction = inverseProjection * direction;
    direction /= direction.w;
    direction = viewZ * direction / direction.z;
    vec4 position = inverseView * vec4(direction.xyz, 1.f);
    
    vec3 cameraPosition = inverseView[3].xyz;
    vec3 wo = normalize(cameraPosition - position.xyz);
    float cosThetao = max(dot(wo, normal), 0.f);

    vec3 f0 = mix(vec3(0.04f), albedo.rgb, metalness);

    // Iterate over lights
    for(uint i = 0; i < batchSize; ++i) {    
        if(lights[i].type == AMBIENT_LIGHT) {
            outFragColor += vec4(occlusion() * lights[i].color * albedo.rgb, albedo.a);
        } else {
            vec3 wi = lightVector(lights[i], position.xyz);
            vec3 hv = normalize(wi + wo);

            vec3 radiance = lights[i].color * attenuation(lights[i], wi, position.xyz) * shadow(lights[i], position, normal);

            float cosThetai = max(dot(wi, normal), 0.f);

            vec3 f = fresnel(clamp(dot(hv, wo), 0.f, 1.f), f0);
            float d = distributionGGX(normal, hv, roughness);
            float g = geometrySmith(cosThetai, cosThetai, roughness);

            vec3 kd = (1.f - f) * (1.f - metalness);
            vec3 lambert = albedo.rgb / PI * kd;

            vec3 cookTorrance = d * f * g / max((4.f * cosThetai * cosThetao), 0.15f);

            outFragColor += vec4((lambert + cookTorrance) * radiance * cosThetai, 1.f);
        }
    }
}