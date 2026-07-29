#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;

struct Light {
    vec2 position;
    vec3 color;
    float radius;
};

uniform Light lights[15];
uniform int lightCount;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2DArray shadow;
uniform sampler2D bloom;
uniform float alpha;

void main() {
    vec3 light = vec3(0.2);
    for(int i = 0; i < lightCount; i++) {
        float dist = length(lights[i].position - gl_FragCoord.xy);
        if(dist < lights[i].radius) {
            float t = clamp(dist / lights[i].radius, 0.0, 1.0);
            float intensity = 1.0 - smoothstep(0.0, 1.0, t);
            float shadow = 1.0 - texture(shadow, vec3(TexCoord, i)).r;
            light += lights[i].color * shadow * intensity;
        }
    }
    vec3 backgroundColor = texture(tex1, TexCoord).rgb * light + texture(bloom, TexCoord).rgb;
    vec3 forgroundColor = texture(tex2, TexCoord).rgb * light;
    vec3 result = mix(backgroundColor, forgroundColor, texture(tex2, TexCoord).a);
    FragColor = vec4(result, alpha);
}