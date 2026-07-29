#version 330 core

uniform sampler2D srcTexture;
uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

in vec2 TexCoord;
layout (location = 0) out vec4 downsample;

float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2.0 * sigma * sigma)) / (2.0 * 3.14159265 * sigma * sigma);
}

void main() {
    //vec2 tex_offset = 1.0 / textureSize(srcTexture, 0); // gets size of single texel
    //vec3 result = texture(srcTexture, TexCoord).rgb * weight[0]; // current fragment's contribution
    //if(horizontal)
    //{
    //    for(int i = 1; i < 5; ++i)
    //    {
    //        result += texture(srcTexture, TexCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
    //        result += texture(srcTexture, TexCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
    //    }
    //}
    //else
    //{
    //    for(int i = 1; i < 5; ++i)
    //    {
    //        result += texture(srcTexture, TexCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
    //        result += texture(srcTexture, TexCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
    //    }
    //}
    //downsample = vec4(result, 1.0);

    vec3 result = vec3(0.0);
    float sum = 0.0;

    vec2 texelSize = 1.0 / textureSize(srcTexture, 0);

    // 2D Kernel
    for (int x = -4; x <= 4; ++x) {
        for (int y = -4; y <= 4; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            float weight = gaussian(length(vec2(x, y)), 3.0);
            result += texture(srcTexture, TexCoord + offset).rgb * weight;
            sum += weight;
        }
    }

    result /= sum; // Normalisierung
    downsample = vec4(result, 1.0);
}