#include <ass3/scene.hpp>
#include <ass3/primitives.hpp>

#include <string>

namespace {
    const char *SKY_PATH = "res/textures/sky/";
}

scene_t make_scene(const GLuint mirror_one_tex, const GLuint mirror_two_tex) {
    model_t skybox = make_skybox(SKY_PATH);

    const glm::vec3 point_light_one_pos = {-25.2f, 4, -10};
    const glm::vec3 point_light_two_pos = {-12.5f, 7, 0};
	const glm::vec3 point_light_three_pos = {-37.7f, 7, 0};
	const glm::vec3 dir_light_pos = {-25.2f, 20, 0};

    node_t mirror_one = {
        node_t::MIRROR,
        make_volume(
            {1, 10, 10},
            {}
        ),
        glm::translate(glm::mat4(1), {10.0f, 3.5f, 0}),
        {},
        mirror_one_tex
    };

    node_t mirror_two = {
        node_t::MIRROR,
        make_volume(
            {1, 10, 10},
            {}
        ),
        glm::translate(glm::mat4(1), {-60.0f, 3.5f, 0}),
        {},
        mirror_two_tex
    };

    node_t cube = {
            node_t::CUBE,
            make_volume(
                    { 1, 1, 1 },
                    { "res/textures/cube/cube_diff.png", "res/textures/cube/cube_spec.png" }
            ),
            glm::mat4(1),
            { }
    };

	node_t obst_one = {
        node_t::OBST,
        make_volume(
				{ 1, 1, 1 },
				{ "res/textures/cube/obst.png", "res/textures/sphere/gold_spec.jpg" }
            ),
            glm::translate(glm::mat4(1), {-12.5f, 0, 0}),
            { }
    };

	node_t obst_two = {
        node_t::OBST,
        make_volume(
				{ 1, 1, 1 },
				{ "res/textures/cube/obst.png", "res/textures/sphere/gold_spec.jpg" }
            ),
            glm::translate(glm::mat4(1), {-37.7f, 0, 0}),
            { }
    };

    node_t sphere = {
            node_t::SPHERE,
            make_sphere(
                    { 500, 500 },
                    { "res/textures/sphere/gold_diff.jpg", "res/textures/sphere/gold_spec.jpg" }
            ),
            glm::translate(glm::mat4(1), { -25, 20, 0 }) *
            glm::scale(glm::mat4(1), { 2, 2, 2 }),
            { }
    };

    node_t torus_one = {
            node_t::TORUS,
            make_torus(
				{ 0.5f, 1.f, 32 },
				{ "res/textures/torus/orange.jpg", "res/textures/torus/marble_spec.jpg" }
            ),
            glm::translate(glm::mat4(1), { -25.2f, 3.0f, 0 })
            * glm::rotate(glm::mat4(1), glm::pi<float>() / 2, { 0, 0, 1 })
			*glm::scale(glm::mat4(1), {6, 2, 4}),
            { }
    };

	node_t torus_two = {
		node_t::TORUS,
		make_torus(
				{ 0.5f, 1.f, 32 },
				{ "res/textures/torus/purple.jpg", "res/textures/torus/marble_spec.jpg" }
		),
		glm::translate(glm::mat4(1), { -50.6f, 3.0f, 0 })
		* glm::rotate(glm::mat4(1), glm::pi<float>() / 2, { 0, 0, 1 })
		*glm::scale(glm::mat4(1), {6, 2, 4}),
		{ }
    };

	node_t torus_three = {
		node_t::TORUS,
		make_torus(
				{ 0.5f, 1.f, 32 },
				{ "res/textures/torus/purple.jpg", "res/textures/torus/marble_spec.jpg" }
		),
		glm::translate(glm::mat4(1), { 0.5f, 3.0f, 0 })
		* glm::rotate(glm::mat4(1), glm::pi<float>() / 2, { 0, 0, 1 })
		*glm::scale(glm::mat4(1), {6, 2, 4}),
		{ }
    };

    material_param_t mtl_light_mesh;
    mtl_light_mesh.emissive = glm::vec3(1, 1, 1);

    node_t light_one_mesh = {
            node_t::LIGHT_MESH,
            make_sphere(
                    { 4, 4 },
                    mtl_light_mesh
            ),
            glm::translate(glm::mat4(1), { point_light_one_pos })
            * glm::scale(glm::mat4(1), { 0.1, 0.1, 0.1 }),
            { }
    };

    node_t light_two_mesh = {
            node_t::LIGHT_MESH,
            make_sphere(
                    { 4, 4 },
                    mtl_light_mesh
            ),
            glm::translate(glm::mat4(1), { point_light_two_pos })
            * glm::scale(glm::mat4(1), { 0.1, 0.1, 0.1 }),
            { }
    };

	node_t light_three_mesh = {
		node_t::LIGHT_MESH,
		make_sphere(
				{ 4, 4 },
				mtl_light_mesh
		),
		glm::translate(glm::mat4(1), { point_light_three_pos })
		* glm::scale(glm::mat4(1), { 0.1, 0.1, 0.1 }),
		{ }
    };

	node_t light_four_mesh = {
		node_t::LIGHT_MESH,
		make_sphere(
				{ 20, 20 },
				mtl_light_mesh
		),
		glm::translate(glm::mat4(1), { dir_light_pos })
		* glm::scale(glm::mat4(1), { 0.5, 0.5, 0.5 }),
		{ }
    };

    point_light_t point_light_one = {
        point_light_one_pos,
        glm::vec3{ 1.0f, 1.0f, 1.0f },
        0.3f
    };

    point_light_t point_light_two = {
        point_light_two_pos,
        glm::vec3{ 1.0f, 0.0f, 0.0f },
        0.3f
    };

	point_light_t point_light_three = {
        point_light_three_pos,
        glm::vec3{ 0.0f, 1.0f, 0.0f },
        0.3f
    };

    directional_light_t dir_light = {
            glm::vec3{ 0.1, 0.1, 0.1},
            glm::vec3{ 0.01, 0.01, 0.01},
            glm::vec3{ 1.0, 1.0, 1.0},
			dir_light_pos
    };

    auto base = node_t{
        node_t::BASE,
        {},
        glm::mat4{1},
    };

    auto floating = node_t{
        node_t::FLOATING,
        {},
        glm::mat4{1},
    };

    auto back_and_forth= node_t{
        node_t::BACKANDFORTH,
        {},
        glm::mat4{1},
    };

    auto spline = node_t{
        node_t::SPLINE,
        {},
        glm::mat4{1},
    };

    auto sphere_anim = node_t{
        node_t::SPHEREANIM,
        {},
        glm::mat4{1},
    };

    sphere_anim.children.push_back(sphere);

    spline.children.push_back(cube);
    // spline.children.push_back(sphere_anim);

    base.children.push_back(spline);
	base.children.push_back(obst_one);
	base.children.push_back(obst_two);

    back_and_forth.children.push_back(torus_one);

    floating.children.push_back(back_and_forth);
	floating.children.push_back(torus_two);
	floating.children.push_back(torus_three);

    base.children.push_back(floating);

    base.children.push_back(light_one_mesh);
	base.children.push_back(light_two_mesh);
	base.children.push_back(light_three_mesh);
	base.children.push_back(light_four_mesh);

    base.children.push_back(sphere);
    base.children.push_back(mirror_one);
    base.children.push_back(mirror_two);

    std::vector<point_light_t> point_lights;
    point_lights.push_back(point_light_one);
	point_lights.push_back(point_light_two);
	point_lights.push_back(point_light_three);


    return scene_t{ point_lights, dir_light, base, skybox };
}
