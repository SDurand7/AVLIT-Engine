#version 410 core

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec3 outPosition;
layout(location = 1) out vec3 outNormal;


uniform mat4 vp;
uniform mat4 model;


void main() {
    outPosition = (model * vec4(inPosition, 1.f)).xyz;
    outNormal = inNormal;

    gl_Position = vp * vec4(outPosition, 1.f);
}
