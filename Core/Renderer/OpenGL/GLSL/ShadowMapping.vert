#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTextureCoord;

layout(location = 0) out vec2 textureCoord;

uniform mat4 mvp;


void main() { 
    gl_Position = mvp * vec4(inPosition, 1.f);
    textureCoord = inTextureCoord;
}