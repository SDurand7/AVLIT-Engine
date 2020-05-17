#version 410 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

layout (location = 0) out vec4 outFragColor;

uniform vec3 color;

// Weird attempt to have some smooth shading on debug models (independently of the lights in the scene)
void main() {
    const vec3 up = vec3(0.f, 1.f, 0.f);
    const vec3 down = -up;
    const vec3 right = vec3(1.f, 0.f, 0.f);
    const vec3 left = -right;
    const vec3 front = vec3(0.f, 0.f, 1.f);
    const vec3 back = -front;

    outFragColor = vec4(color
                        + color * max(dot(inNormal, up), 0.1f)
                        + color * max(dot(inNormal, down), 0.1f)
                        + color * max(dot(inNormal, right), 0.1f)
                        + color * max(dot(inNormal, left), 0.1f)
                        + color * max(dot(inNormal, front), 0.1f)
                        + color * max(dot(inNormal, back), 0.1f), 1.f);
}