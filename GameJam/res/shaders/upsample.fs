#version 330 core
uniform sampler2D srcTexture1;
uniform sampler2D srcTexture2;

in vec2 TexCoord;
layout (location = 0) out vec4 upsample;

void main() {
    upsample = vec4(texture(srcTexture1, TexCoord).rgb + texture(srcTexture2, TexCoord).rgb, 1.0);
}