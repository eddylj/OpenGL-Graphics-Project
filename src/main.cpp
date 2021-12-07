#include <cstdlib>
#include <chrono>
#include <thread>
#include <iostream>
#include <stack>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chicken3421/chicken3421.hpp>

#include <ass3/memes.hpp>
#include <ass3/scene.hpp>
#include <ass3/renderer.hpp>
#include <ass3/animator.hpp>
#include <ass3/framebuffer.hpp>
#include <ass3/camera.hpp>

struct {
    int WIDTH = 1280;
    int HEIGHT = 720;
    const char *TITLE = "OWO";
    float ASPECT = (float) WIDTH / (float) HEIGHT;
} win_opts;

/**
 * Returns the difference in time between when this function was previously called and this call.
 * @return A float representing the difference between function calls in seconds.
 */
float time_delta();

/**
 * Returns the current time in seconds.
 * @return Returns the current time in seconds.
 */
float time_now();

int main() {
    GLFWwindow *win = marcify(chicken3421::make_opengl_window(win_opts.WIDTH, win_opts.HEIGHT, win_opts.TITLE));
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    camera_t cam = make_camera({ -25.2f, 10, -35 }, { -25.2f, 10, 10 }, 0);
    
    framebuffer_t mirror_one_framebuffer = make_framebuffer(win_opts.WIDTH, win_opts.HEIGHT);
    framebuffer_t mirror_two_framebuffer = make_framebuffer(win_opts.WIDTH, win_opts.HEIGHT);

    scene_t scene = make_scene(mirror_one_framebuffer.texture, mirror_two_framebuffer.texture);

    framebuffer_t framebuffer = make_framebuffer(win_opts.WIDTH, win_opts.HEIGHT);
    framebuffer_t hdr_framebuffer = make_framebuffer(win_opts.WIDTH, win_opts.HEIGHT);
    model_t hdr_screen_quad = make_screen_quad(framebuffer.texture);
    model_t screen_quad = make_screen_quad(hdr_framebuffer.texture);
    

    animator_t animator = make_animator();
    renderer_t renderer = make_renderer(glm::perspective(glm::pi<float>() / 3, win_opts.ASPECT, 0.1f, 100.f));

    while (!glfwWindowShouldClose(win)) {
        float dt = time_delta();

        update_camera(cam, win, dt);

        float time = time_now();
        int kern = check_cube_pos(time);
        animate(animator, scene, time);

        GLuint tex = render_reflection(renderer, scene, glm::vec3(-25, 20, 0));

        glBindFramebuffer(GL_FRAMEBUFFER, mirror_one_framebuffer.fbo);
        camera_t mirror_one_cam = make_camera({-53.0f, 3.5f, 0}, {-50, 3.5f, 0}, 2);
        render(renderer, scene, mirror_one_cam, false, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, mirror_two_framebuffer.fbo);
        // camera_t mirror_two_cam = make_camera({4.0f, 3.5f, 0}, {3.0f, 3.5f, 0}, 2);
        render(renderer, scene, mirror_one_cam, false, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fbo);
        render(renderer, scene, cam, true, tex);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, hdr_framebuffer.fbo);
        post_process_hdr(renderer, hdr_screen_quad);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        post_process_hdr(renderer, screen_quad);




        post_process(renderer, screen_quad, kern);

        glfwSwapBuffers(win);
        glfwPollEvents();

        // not entirely correct as a frame limiter, but close enough
        // it would be more correct if we knew how much time this frame took to render
        // and calculated the distance to the next "ideal" time to render and only slept that long
        // the current way just always sleeps for 16.67ms, so in theory we'd drop frames
        std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000.f / 60));
    }

    // deleting the whole window also removes the opengl context, freeing all our memory in one fell swoop.
    chicken3421::delete_opengl_window(win);

    return EXIT_SUCCESS;
}

float time_delta() {
    static float then = time_now();
    float now = time_now();
    float dt = now - then;
    then = now;
    return dt;
}

float time_now() {
    return (float) glfwGetTime();
}