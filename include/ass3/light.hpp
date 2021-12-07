#ifndef COMP3421_TUTORIAL_08_LIGHT_HPP
#define COMP3421_TUTORIAL_08_LIGHT_HPP

#include <glm/glm.hpp>

struct directional_light_t {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 position;
};

struct point_light_t {
    glm::vec3 pos;
    glm::vec3 color;
    float attenuation;
};

#endif //COMP3421_TUTORIAL_08_LIGHT_HPP
