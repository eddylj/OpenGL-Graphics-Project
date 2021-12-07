#version 330 core

uniform sampler2D u_scene;

in vec2 v_tc;

out vec4 f_color;

const float gamma  = 2.2;
const float exposure = 0.3;

void main(void) {

    vec3 original_color = texture(u_scene, v_tc).rgb;

    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-original_color * exposure);
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

    // in chromatography, L means grayscale, or black and white.

    f_color = vec4(mapped, 1.0);
}
