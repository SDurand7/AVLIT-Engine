#version 460 core

layout(location = 0) in vec2 textureCoord;

layout(location = 0) out float occlusion;


uniform sampler2D normalZMap;
uniform sampler2D noise;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 inverseProjection;

uniform int width;
uniform int height;

uniform vec3 samples[64];


void main() {
    vec2 scale = vec2(width, height) / textureSize(noise, 0);
    vec3 noise = vec3(texture(noise, textureCoord * scale).xy, 0.f);

    vec3 normal = (view * vec4(texture(normalZMap, textureCoord).xyz, 0.f)).xyz;
    vec3 tangent = normalize(noise - normal * dot(normal, noise));
    vec3 bitangent = cross(normal, tangent);

    mat3 tbn = mat3(tangent, bitangent, normal);

    float viewZ = texture(normalZMap, textureCoord).w;
    vec4 direction = inverseProjection * vec4(2.f * textureCoord - 1.f, 0.f, 1.f);
    direction /= direction.w;
    direction /= direction.z;
    vec3 position = viewZ * direction.xyz;

    occlusion = 0.f;
    for(int i = 0; i < 64; ++i) {
        float radius = 1.f;

        vec3 sampleView = radius * tbn * samples[i] + position;
        vec4 sampleNDC = projection * vec4(sampleView, 1.f);
        sampleNDC /= sampleNDC.w;
        sampleNDC = sampleNDC * 0.5f + 0.5f;

        float sampleDepth = texture(normalZMap, sampleNDC.xy).w;

        // Range check
        occlusion += float(sampleDepth > sampleView.z) * float(abs(position.z - sampleDepth) < radius);
    }

    occlusion = 1 - occlusion / 64;
}
