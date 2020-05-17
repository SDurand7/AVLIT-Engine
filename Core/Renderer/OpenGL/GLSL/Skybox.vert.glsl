#version 410 core

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 outTexCoords;


uniform mat4 vp;


void main() {
    outTexCoords = inPosition;
    gl_Position = (vp * vec4(inPosition, 1.0f)).xyww;
}
