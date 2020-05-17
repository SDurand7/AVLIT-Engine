#version 410 core

layout(location = 0) in vec2 textureCoord;

layout(location = 0) out vec4 outFragColor;


uniform sampler2D hdrColor;


void main() {
    // Reinhard
    vec3 color = texture(hdrColor, textureCoord).xyz;

    color /= color + 1.f;
    color = pow(color, vec3(1 / 2.2f));

    outFragColor = vec4(color, 1.f);
}