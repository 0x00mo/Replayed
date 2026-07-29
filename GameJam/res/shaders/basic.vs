#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;

layout(binding = 0, std430) readonly buffer ssbo1 {
    mat4 modelMatrices[];
};

out vec2 TexCoord;
flat out int InstanceID;

void main() {
    gl_Position = projection * view * modelMatrices[gl_InstanceID] * vec4(position, 1.0);
    TexCoord = texCoord;
    InstanceID = gl_InstanceID;
}