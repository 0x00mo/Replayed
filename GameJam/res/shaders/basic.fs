#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
flat in int InstanceID;

layout(binding = 1, std430) readonly buffer ssbo2 {
    vec4 colors[];
};

struct Sprite {
	int x;
	int y;
    int w;
	int h;
};

layout(binding = 2, std430) readonly buffer ssbo3 {
    Sprite sprites[];
};

uniform sampler2D textureAtlas;

void main() {
    const float atlasSize = 512.0f;
    Sprite sprite = sprites[InstanceID];

    vec3 tex = texture(textureAtlas, vec2((TexCoord.x * sprite.w + sprite.x) / atlasSize, (TexCoord.y * sprite.h + sprite.y) / atlasSize)).rgb;
    float alpha = texture(textureAtlas, vec2((TexCoord.x * sprite.w + sprite.x) / atlasSize, (TexCoord.y * sprite.h + sprite.y) / atlasSize)).a;
    vec3 color = colors[InstanceID].rgb;

    FragColor = vec4(tex * color, alpha);
}