#version 460 core

layout(location = 0) in vec3 inTexCoords;

layout(location = 0) out vec4 fragColor;


uniform samplerCube skybox;


void main() { fragColor = texture(skybox, inTexCoords); }