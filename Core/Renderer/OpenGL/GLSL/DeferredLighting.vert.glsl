#version 410 core

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec2 textureCoord;


void main() {
    gl_Position = vec4(inPosition, 1.f);

    textureCoord = 0.5f * inPosition.xy + 0.5f;
}