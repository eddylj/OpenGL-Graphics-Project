#ifndef COMP3421_TUTORIAL_05_ANIMATOR_HPP
#define COMP3421_TUTORIAL_05_ANIMATOR_HPP

#include <glm/ext.hpp>

#include <ass3/scene.hpp>

struct bspline_t {
    std::vector<glm::vec3> s1;
    std::vector<glm::vec3> s2;
};

/**
 * A type representing a figure-8, or lemniscate.
 * Unremarkable.
 */
struct lemnisacte_t {};

/**
 * Our primitive animator only can draw lemniscates.
 *
 */
struct animator_t {
    bspline_t spline_animation;
    lemnisacte_t lemniscate_animation;
};

/**
 * Given the current animation context animator and time, apply animations to the scene.
 *
 * Currently, the only two animations applied are figure-8s.
 *
 * @param animator The animation context
 * @param scene The scene to animate
 * @param now The current time in seconds.
 */
void animate(const animator_t &animator, scene_t &scene, float now);

int check_cube_pos(float now);

/**
 * Initialise the animation context
 * @return A fresh animator
 */
animator_t make_animator();

#endif //COMP3421_TUTORIAL_05_ANIMATOR_HPP
