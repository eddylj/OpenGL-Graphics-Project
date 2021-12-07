#version 330 core

uniform mat4 u_v;
uniform mat4 u_p;
uniform mat4 u_m;

layout (location = 0) in vec3 a_pos;
layout (location = 2) in vec2 aTexCoord;

out vec2 vTexCoord;

void main(void) {
    mat4 mv = u_v * u_m;
    vec4 view_pos = mv * vec4(a_pos, 1);

    gl_Position = u_p * view_pos;

    vTexCoord = aTexCoord * 1/9 + vec2(0.2);
}
