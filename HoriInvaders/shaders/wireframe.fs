#version 450 core

out vec4 outColor;

uniform vec3 color;

void main() {
    outColor = vec4(vec3(0.0f, 1.0f, 0.0f), 1);
}