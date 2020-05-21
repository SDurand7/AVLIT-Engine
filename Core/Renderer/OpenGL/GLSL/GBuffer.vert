#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTextureCoord;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

out Data {
    float viewZ;
    vec2 textureCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
} vs_out;

uniform mat4 projection;
uniform mat4 modelView;
uniform mat4 model;


void main() {
    gl_Position = modelView * vec4(inPosition, 1.f);
    vs_out.viewZ = gl_Position.z;
    gl_Position = projection * gl_Position;

    vs_out.textureCoord = inTextureCoord;

    // Incorrect for non uniform scaling
    vs_out.normal = normalize((model * vec4(inNormal, 0.f)).xyz);
    vs_out.tangent = normalize((model * vec4(inTangent, 0.f)).xyz);
    vs_out.bitangent = normalize((model * vec4(inBitangent, 0.f)).xyz);
}