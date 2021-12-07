#version 330 core

uniform sampler2D u_scene;

in vec2 v_tc;

out vec4 f_color;

const float offset = 1.0 / 300.0;
const float gamma  = 2.2;

void main(void) {

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );


    vec3 sampleTex[9];

    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(u_scene, v_tc.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    vec3 original_color = texture(u_scene, v_tc).rgb;

    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-col * 0.2);
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

    // in chromatography, L means grayscale, or black and white.
    // float L = (original_color.r + original_color.g + original_color.b) / 3;

    // f_color = vec4(col, 1);
    // f_color = vec4(original_color.r, original_color.g, original_color.b, 1);
    f_color = vec4(mapped, 1.0);
}
