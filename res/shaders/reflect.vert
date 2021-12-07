#version 330 core

uniform mat4 u_v;
uniform mat4 u_p;
uniform mat4 u_m;

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_tex_coord;

out vec3 v_tex_coord;

void main(void) {
    mat4 mv = u_v * u_m;

    vec4 view_pos = mv * vec4(a_pos, 1);
    // v_tex_coord = inverse(mat3(u_v)) * reflect(normalize(view_pos.xyz), mat3(mv) * a_normal);
    v_tex_coord = a_tex_coord;
    gl_Position = u_p * view_pos;
}
