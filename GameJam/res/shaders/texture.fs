#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex;
uniform vec3 color;
uniform float alpha;

void main() {
    FragColor = vec4(texture(tex, TexCoord).rgb * color, alpha);
}