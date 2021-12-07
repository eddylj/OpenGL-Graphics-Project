#include <ass3/animator.hpp>

#include <iostream>

#include <cmath>
#include <stack>

// Repeat each animation every 4 jump cycles
const float PERIOD = 4 * M_PI;

/**
 * Calculate a transformation matrix to simulate a cube 
 * jumping using sin and cos parameters.
 * @param lemni A tag type to represent a lemniscate
 * @param now The time now in seconds
 * @return A transformation matrix
 */
glm::mat4 animate(const lemnisacte_t &lemni, float now) {
    auto t = std::fmod(now, PERIOD);

    auto x = 4 * t;
    auto y = std::sin(-2 * t - 3* M_PI/2) * 3.5;
    float a = (-std::cos(2 * t - 3* M_PI/2) + 1) / 2;

    if (y < 0) {
        y = 0;
        a = 0;
    }
    auto z = 0;

    return glm::translate(glm::mat4(1), glm::vec3{ -x, y, z }) * glm::rotate(glm::mat4(1), glm::radians(a * 360), glm::vec3(0, 0, 0.1));
}

int check_cube_pos(float now) {
    auto t = std::fmod(now, PERIOD);

    auto x = 4 * t;
    auto y = std::sin(-2 * t - 3* M_PI/2) * 3.5;
    float a = (-std::cos(2 * t - 3* M_PI/2) + 1) / 2;

    if (y < 0) {
        y = 0;
        a = 0;
    }
    if (-x < -24 and -x > -26) {
        return 1;
    }
    else {
        return 0;
    }
}


animator_t make_animator() {
    std::vector<glm::vec3> s1 = {
        {0, 0, 0},
        {1, 0, 1.5},
        {1, 0, -1.5},
        {0, 0, 0},
    };

    std::vector<glm::vec3> s2 = {
        {0, 0, 0},
        {-1.85, 0, 1.5},
        {-1.85, 0, -1.5},
        {0, 0, 0},
    };

    float const SCALE = 40;

    for (auto && cp: s1) {
        cp *= SCALE;
    }

    for (auto && cp: s2) {
        cp *= SCALE;
    }

    return animator_t{ bspline_t{s1, s2 }, lemnisacte_t{ }};
}

void animate(const animator_t &animator, scene_t &scene, float now) {
    std::stack<node_t *> animatables{{ &scene.root }};
    while (!animatables.empty()) {
        node_t *n = animatables.top();
        animatables.pop();

        switch (n->kind) {
            case node_t::SPHEREANIM:
                n->transform *= glm::rotate(glm::mat4(1.0), glm::radians(1.0f) * now * glm::sin(now), glm::vec3(0, 0.1, 0));
                break;

            case node_t::SPLINE:
                n->transform = animate(animator.lemniscate_animation, now);
                break;

            case node_t::FLOATING:
                n->transform = glm::translate(glm::mat4{1}, {0, glm::sin(now), 0});
                break;

            case node_t::BACKANDFORTH:
                n->transform = glm::translate(glm::mat4{1}, {glm::cos(now)/3, 0, 0});
                break;

            default:
                break;
        }

        for (node_t &c: n->children) {
            animatables.push(&c);
        }
    }
}
