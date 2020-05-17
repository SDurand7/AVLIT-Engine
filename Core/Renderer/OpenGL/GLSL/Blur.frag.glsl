#version 410 core

layout(location = 0) in vec2 textureCoord;

layout(location = 0) out float blurred;


uniform sampler2D toBlur;


void main() {
    vec2 texelSize = 1.f / textureSize(toBlur, 0);
    float result = 0.f;

    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            result += texture(toBlur, textureCoord + texelSize * vec2(i - 1.5f, j - 1.5f)).x;
        }
    }

    blurred = result / 16;
}
