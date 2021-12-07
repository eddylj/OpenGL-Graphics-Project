#ifndef COMP3421_TUTORIAL_08_RENDERER_HPP
#define COMP3421_TUTORIAL_08_RENDERER_HPP

#include <glad/glad.h>
#include <glm/ext.hpp>

#include <ass3/primitives.hpp>
#include <ass3/scene.hpp>

/**
 * A simple bag of all the OpenGL rendering bits we've used up until now.
 */
struct renderer_t {
    glm::mat4 projection;

    GLuint program;

    // vs uniforms
    GLint p_loc;
    GLint v_loc;
    GLint m_loc;

    // fs uniforms
    GLint diff_map_loc;
    GLint spec_map_loc;

    GLint diffuse_loc;
    GLint specular_loc;
    GLint emissive_loc;
    GLint shininess_loc;

    GLuint prog_skybox;
    GLuint prog_filter;
    GLuint prog_reflect;
    GLuint prog_mirror;
    GLuint prog_kern_filter;
};

/**
 * Initialise our renderer.
 *
 * Inside of this we can load shaders, grab uniform locations, etc.
 * Of course, if we have different shaders, it is conceivable we would have different capabilities enabled/disabled too.
 * Whatever doesn't change can be initialised here, then.
 *
 * @param projection The projection matrix to use (almost always a perspective transform)
 * @return The renderer.
 */
renderer_t make_renderer(const glm::mat4 &projection);

GLuint render_reflection(const renderer_t &renderer, const scene_t &scene, const glm::vec3 position);



/**
 * Renders the current scene specified by scene.
 *
 * Starting at the root of the scenegraph, this function will "recursively" descend the scenegraph and draw all nodes.
 * Nothing is modified except the OpenGL context.
 *
 * @param renderer the rendering context gotten from make_renderer()
 * @param scene the scene to draw.
 */
void render(const renderer_t &renderer, const scene_t &scene, const camera_t &cam, const bool incl_mirror, GLuint cubemap);
void post_process_hdr(const renderer_t &renderer, const model_t &screen_quad);

void post_process(const renderer_t &renderer, const model_t &screen_quad, int kernel_mode);

#endif //COMP3421_TUTORIAL_08_RENDERER_HPP
