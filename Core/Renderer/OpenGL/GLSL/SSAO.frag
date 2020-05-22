#version 460 core

layout(location = 0) in vec2 textureCoord;

layout(location = 0) out float occlusion;


uniform sampler2D normalZ;
uniform sampler2D noise;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 inverseProjection;

uniform int width;
uniform int height;

uniform vec3 samples[64];


void main() {
    vec2 scale = vec2(width, height) / textureSize(noise, 0);
    vec3 noiseVec = texture(noise, textureCoord * scale).xyz;

    vec3 normal = (view * vec4(texture(normalZ, textureCoord).xyz, 0.f)).xyz;
    vec3 tangent = normalize(noiseVec - normal * dot(normal, noiseVec));
    vec3 bitangent = cross(normal, tangent);

    mat3 tbn = mat3(tangent, bitangent, normal);

    float z = texture(normalZ, textureCoord).w;
    vec4 dir = inverseProjection * vec4(2.f * textureCoord - 1.f, 0.f, 1.f);
    dir /= dir.w;
    dir /= dir.z;
    vec3 position = z * dir.xyz;

    float occlusionFactor = 0.f;
    for(int i = 0; i < 64; ++i) {
        float radius = 1.f;

        vec3 sampleView = radius * tbn * samples[i] + position;
        vec4 sampleNDC = projection * vec4(sampleView, 1.f);
        sampleNDC.xy /= sampleNDC.w;
        sampleNDC.xy = sampleNDC.xy * 0.5f + 0.5f;

        float sampledDepth = texture(normalZ, sampleNDC.xy).w;
        // With range check
        occlusionFactor += float(sampledDepth > sampleView.z) * float(abs(position.z - sampledDepth) < radius);
    }

    occlusion = 1 - occlusionFactor / 64;
}
