#version 330 core

in vec2 vTexCoord;

uniform sampler2D u_texture;

out vec4 f_color;

void main() {
    f_color = texture(u_texture, vTexCoord);
}
