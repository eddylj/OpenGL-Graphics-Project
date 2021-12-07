#ifndef COMP3421_TUTORIAL_08_SCENE_HPP
#define COMP3421_TUTORIAL_08_SCENE_HPP

#include <glad/glad.h>

#include <glm/ext.hpp>

#include <ass3/camera.hpp>
#include <ass3/light.hpp>
#include <ass3/primitives.hpp>


struct node_t {
    /**
     * The different types of models in our scene.
     */
    enum {
        CUBE,
        SPHERE,
        TORUS,
        LIGHT_MESH,
        OBST,
        BASE,
        FLOATING,
        SPLINE,
        SPHEREANIM,
        BACKANDFORTH,
        MIRROR
    } kind;
    model_t obj;
    glm::mat4 transform;
    std::vector<node_t> children;
    GLuint texture;
};

/**
 * Our scene consists of a simple camera and the graph of all objects.
 *
 * Note that every node is the same size because all the models are the same size.
 * This isn't necessarily true in the real world.
 * How might one deal with different node sizes?
 */
struct scene_t {
    std::vector<point_light_t> point_lights;
    directional_light_t dir_light;

    node_t root;
    model_t skybox;
};

/**
 * Create our scene in whatever fashion we desire.
 *
 * It is important to note that this function takes the role of a level loader.
 * Thus, we'll almost always be changing this when we want different things in-scene.
 *
 * @return Our scene
 */
scene_t make_scene(const GLuint mirror_one_tex, const GLuint mirror_two_tex);



#endif //COMP3421_TUTORIAL_08_SCENE_HPP
