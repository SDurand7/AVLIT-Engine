#version 410 core

#include "Light.glsl"

layout(location = 0) in vec2 textureCoord;

layout(location = 0) out vec4 outFragColor;


uniform sampler2D normalZ;
uniform sampler2D ambientColor;
uniform sampler2D diffuseColor;
uniform sampler2D specularColor;
uniform sampler2D specularParameter;

uniform sampler2D occlusion;

uniform sampler2DShadow shadowMap;
// Debug stuff
uniform sampler2D depthMap;

uniform samplerCube skybox;

uniform mat4 lightVP;

uniform mat4 inverseView;
uniform mat4 inverseProj;

uniform Light l;


vec3 computeDiffuse(vec3 lightV, vec3 position, vec3 normal) {
    return max(dot(normal, lightV), 0.f) * texture(diffuseColor, textureCoord).xyz;
}

vec3 computeSpecular(vec3 lightV, vec3 position, vec3 normal) {
    vec3 halfV = normalize(lightV + normalize(inverseView[3].xyz - position));
    vec2 specularParameter = texture(specularParameter, textureCoord).xy;
    float specularCoef = specularParameter.y * pow(max(dot(halfV, normal), 0.f), specularParameter.x);

    return specularCoef * texture(skybox, reflect(-lightV, normal)).xyz * texture(specularColor, textureCoord).xyz;
}

float shadow(vec4 position, vec3 normal, vec3 lightV) {
    position.xyz += normal * 0.125f;
    position = lightVP * position;
    position /= position.w;
    position = position * 0.5f + 0.5f;
    vec2 texelSize = 1.f / textureSize(shadowMap, 0);

    float result = 0.f;
    for(float i = -1.5f; i <= 1.5f + 0.001f; i += 1.f) {
        for(float j = -1.5f; j <= 1.5f + 0.001f; j += 1.f) {
            result += texture(shadowMap, vec3(position.xy + vec2(i, j) * texelSize, position.z));
        }
    }

    return result / 16;
}


void main() {
    float alpha = texture(ambientColor, textureCoord).a;

    if(l.type == AMBIENT_LIGHT) {
        outFragColor =
            vec4(texture(occlusion, textureCoord).x * l.color * texture(ambientColor, textureCoord).xyz, alpha);
    } else {
        vec3 normal = texture(normalZ, textureCoord).xyz;
        float z = texture(normalZ, textureCoord).w;
        vec4 dir = inverseProj * vec4(2.f * textureCoord - 1.f, 0.f, 1.f);
        dir /= dir.w;
        dir /= dir.z;
        vec3 position = (inverseView * vec4(z * dir.xyz, 1.f)).xyz;

        vec3 lightV = lightVector(l, position.xyz);

        outFragColor =
            vec4(shadow(vec4(position, 1.f), normal, lightV) * attenuation(l, lightV, position.xyz) * l.color *
                     (computeDiffuse(lightV, position.xyz, normal) + computeSpecular(lightV, position.xyz, normal)),
                 alpha);
    }
}