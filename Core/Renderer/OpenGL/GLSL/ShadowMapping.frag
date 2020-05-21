#version 460 core

layout(location = 0) in vec2 textureCoord;


uniform bool hasAlphaMap;
uniform sampler2D alphaMap;
uniform float alpha;

void main() {
    if((hasAlphaMap ? texture(alphaMap, textureCoord).x : alpha) < 0.4f)
        discard;
}
