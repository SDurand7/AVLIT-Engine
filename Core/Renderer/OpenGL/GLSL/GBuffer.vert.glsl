#version 410 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

layout(location = 0) out vec4 texCoordViewZNormalX;
layout(location = 1) out vec4 tangentNormalY;
layout(location = 2) out vec4 bitangentNormalZ;


uniform mat4 projection;
uniform mat4 modelView;
uniform mat4 model;


void main() {
    gl_Position = modelView * vec4(inPosition, 1.f);
    float viewZ = gl_Position.z;
    gl_Position = projection * gl_Position;

    // Incorrect for non uniform scaling
    vec3 normal = normalize(vec3(model * vec4(inNormal, 0.f)));

    // Pack the normal into the other buffers
    texCoordViewZNormalX = vec4(inTexCoord, viewZ, normal.x);
    tangentNormalY = vec4(normalize(vec3(model * vec4(inTangent, 0.f))), normal.y);
    bitangentNormalZ = vec4(normalize(vec3(model * vec4(inBitangent, 0.f))), normal.z);
}