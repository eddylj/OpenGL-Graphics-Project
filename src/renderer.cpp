#include <ass3/renderer.hpp>
#include <ass3/primitives.hpp>
#include <ass3/framebuffer.hpp>

#include <stack>

#include <chicken3421/chicken3421.hpp>
#include <iostream>
#include <glm/glm.hpp>

namespace {

    const char *VERT_PATH = "res/shaders/default.vert";
    const char *FRAG_PATH = "res/shaders/default.frag";

    const char *SKYBOX_VERT_PATH = "res/shaders/skybox.vert";
    const char *SKYBOX_FRAG_PATH = "res/shaders/skybox.frag";

    const char *FILTER_VERT_PATH = "res/shaders/filter.vert";
    const char *FILTER_FRAG_PATH = "res/shaders/filter.frag";

    const char *REFLECT_VERT_PATH = "res/shaders/reflect.vert";
    const char *REFLECT_FRAG_PATH = "res/shaders/reflect.frag";

    const char *MIRROR_VERT_PATH = "res/shaders/mirror.vert";
    const char *MIRROR_FRAG_PATH = "res/shaders/mirror.frag";

    const char *FILTER_KERN_VERT_PATH = "res/shaders/filterkern.vert";
    const char *FILTER_KERN_FRAG_PATH = "res/shaders/filterkern.frag";

    GLuint load_program(const std::string &vs_path, const std::string &fs_path) {
        using namespace chicken3421;
        GLuint vs = make_shader(vs_path, GL_VERTEX_SHADER), fs = make_shader(fs_path, GL_FRAGMENT_SHADER);
        GLuint prog = make_program(vs, fs);
        delete_shader(vs), delete_shader(fs);
        return prog;
    }

    void
    draw(const renderer_t &renderer, const glm::mat4 &p, const glm::mat4 &v, const glm::mat4 &m, const model_t &model) {
        const auto &mesh = model.mesh;
        const auto &mtl = model.material;

        glUseProgram(renderer.program);

        // set material parameters
        glUniform3fv(renderer.diffuse_loc, 1, glm::value_ptr(mtl.diffuse));
        glUniform3fv(renderer.specular_loc, 1, glm::value_ptr(mtl.specular));
        glUniform3fv(renderer.emissive_loc, 1, glm::value_ptr(mtl.emissive));
        glUniform1f(renderer.shininess_loc, mtl.shininess);

        // set the diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mtl.diff_map);

        // set the spec map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mtl.spec_map);

        glUniformMatrix4fv(renderer.p_loc, 1, GL_FALSE, glm::value_ptr(p));
        glUniformMatrix4fv(renderer.v_loc, 1, GL_FALSE, glm::value_ptr(v));
        glUniformMatrix4fv(renderer.m_loc, 1, GL_FALSE, glm::value_ptr(m));

        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

        glDrawElements(GL_TRIANGLES, (GLsizei) mesh.nverts, index_type, nullptr);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void draw_sky(const renderer_t &renderer, const glm::mat4 &p, const glm::mat4 &v, const model_t &skybox) {
        
        glUseProgram(renderer.prog_skybox);
        glFrontFace(GL_CW);
        glDepthMask(GL_FALSE);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.material.diff_map);

        glBindVertexArray(skybox.mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, skybox.mesh.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox.mesh.ibo);

        chicken3421::set_uniform(renderer.prog_skybox, "u_v", v);
        chicken3421::set_uniform(renderer.prog_skybox, "u_p", p);

        glDrawElements(GL_TRIANGLES, (GLsizei) skybox.mesh.nverts, index_type, nullptr);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDepthMask(GL_TRUE);
        glFrontFace(GL_CCW);
        glUseProgram(0);
    }

    void draw_mirror(const renderer_t &renderer, const glm::mat4 &p, const glm::mat4 &v, const glm::mat4 &m, const model_t &model, GLuint texture) {
        glUseProgram(renderer.prog_mirror);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        chicken3421::set_uniform(renderer.prog_mirror, "u_m", m);
        chicken3421::set_uniform(renderer.prog_mirror, "u_v", v);
        chicken3421::set_uniform(renderer.prog_mirror, "u_p", p);

        glBindVertexArray(model.mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, model.mesh.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.mesh.ibo);
        glDrawElements(GL_TRIANGLES, (GLsizei) model.mesh.nverts, index_type, nullptr);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void draw_reflective(const renderer_t &renderer, const glm::mat4 &p, const glm::mat4 &v, const glm::mat4 &m, const model_t &model, GLuint texture) {
        glUseProgram(renderer.prog_reflect);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        chicken3421::set_uniform(renderer.prog_reflect, "u_m", m);
        chicken3421::set_uniform(renderer.prog_reflect, "u_v", v);
        chicken3421::set_uniform(renderer.prog_reflect, "u_p", p);

        glBindVertexArray(model.mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, model.mesh.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.mesh.ibo);
        glDrawElements(GL_TRIANGLES, (GLsizei) model.mesh.nverts, index_type, nullptr);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}

renderer_t make_renderer(const glm::mat4 &projection) {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint render_program = load_program(VERT_PATH, FRAG_PATH);

    GLint p_loc = chicken3421::get_uniform_location(render_program, "p");
    GLint v_loc = chicken3421::get_uniform_location(render_program, "v");
    GLint m_loc = chicken3421::get_uniform_location(render_program, "m");

    GLint diff_map_loc = chicken3421::get_uniform_location(render_program, "u_diff_map");
    GLint spec_map_loc = chicken3421::get_uniform_location(render_program, "u_spec_map");

    GLint diffuse_loc = chicken3421::get_uniform_location(render_program, "u_diffuse");
    GLint specular_loc = chicken3421::get_uniform_location(render_program, "u_specular");
    GLint emissive_loc = chicken3421::get_uniform_location(render_program, "u_emissive");
    GLint shininess_loc = chicken3421::get_uniform_location(render_program, "u_shininess");

    GLuint prog_skybox = load_program(SKYBOX_VERT_PATH, SKYBOX_FRAG_PATH);
    GLuint prog_filter = load_program(FILTER_VERT_PATH, FILTER_FRAG_PATH);
    GLuint prog_reflect = load_program(REFLECT_VERT_PATH, REFLECT_FRAG_PATH);
    GLuint prog_mirror = load_program(MIRROR_VERT_PATH, MIRROR_FRAG_PATH);
    GLuint prog_kern_filter = load_program(FILTER_KERN_VERT_PATH, FILTER_KERN_FRAG_PATH);

    return { projection,
             render_program,
             p_loc, v_loc, m_loc,
             diff_map_loc, spec_map_loc,
             diffuse_loc, specular_loc, emissive_loc, shininess_loc,
             prog_skybox, prog_filter, prog_reflect, prog_mirror, prog_kern_filter
    };
}


GLuint render_reflection(const renderer_t &renderer, const scene_t &scene, const glm::vec3 position) {
    GLuint cubemap = chicken3421::make_texture();
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (unsigned int side = 0; side < 6; side++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, 0, GL_RGBA8, 256, 256, 0, GL_RGBA8, GL_UNSIGNED_BYTE, NULL);
    }

    framebuffer_t framebuffer = make_framebuffer(1280, 720);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    GLuint renderbuffer = chicken3421::make_renderbuffer();
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 256, 256);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebuffer.fbo);
    
    std::vector<glm::vec3> targets;
    targets.push_back(position + glm::vec3(1.0f, 0.0f, 0.0f));
    targets.push_back(position + glm::vec3(-1.0f, 0.0f, 0.0f));
    targets.push_back(position + glm::vec3(0.0f, 1.0f, 0.0f));
    targets.push_back(position + glm::vec3(0.0f, -1.0f, 0.0f));
    targets.push_back(position + glm::vec3(0.0f, 0.0f, 1.0f));
    targets.push_back(position + glm::vec3(0.0f, 0.0f, -1.0f));

    // Render scene to cubemap
    glViewport(0, 0, 1280, 720);

    for (unsigned int side = 0; side < 6; side++)
    {

        camera_t capture_cam = make_camera(position, targets[side], 0);
        
        GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + side;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, face, cubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render(renderer, scene, capture_cam, false, 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return cubemap;
}


void render(const renderer_t &renderer, const scene_t &scene, const camera_t &cam, const bool incl_mirror, GLuint cubemap) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 p = renderer.projection;
    glm::mat4 v = get_view(cam);

    draw_sky(renderer, p, v, scene.skybox);

    glUniform1i(renderer.diff_map_loc, 0);
    glUniform1i(renderer.spec_map_loc, 1);

    glUseProgram(renderer.program);

    // set up the lighting and send all parameters to frag shader
    glUniform3fv(chicken3421::get_uniform_location(renderer.program, "point_lights[0].u_light_pos"), 1, glm::value_ptr(scene.point_lights[0].pos));
    glUniform3fv(chicken3421::get_uniform_location(renderer.program, "point_lights[0].u_light_color"), 1, glm::value_ptr(scene.point_lights[0].color));
    glUniform1f(chicken3421::get_uniform_location(renderer.program, "point_lights[0].u_light_attenuation"), scene.point_lights[0].attenuation);

    glUniform3fv(chicken3421::get_uniform_location(renderer.program, "point_lights[1].u_light_pos"), 1, glm::value_ptr(scene.point_lights[1].pos));
    glUniform3fv(chicken3421::get_uniform_location(renderer.program, "point_lights[1].u_light_color"), 1, glm::value_ptr(scene.point_lights[1].color));
    glUniform1f(chicken3421::get_uniform_location(renderer.program, "point_lights[1].u_light_attenuation"), scene.point_lights[1].attenuation);

    glUniform3fv(chicken3421::get_uniform_location(renderer.program, "point_lights[2].u_light_pos"), 1, glm::value_ptr(scene.point_lights[2].pos));
    glUniform3fv(chicken3421::get_uniform_location(renderer.program, "point_lights[2].u_light_color"), 1, glm::value_ptr(scene.point_lights[2].color));
    glUniform1f(chicken3421::get_uniform_location(renderer.program, "point_lights[2].u_light_attenuation"), scene.point_lights[2].attenuation);

    glUniform3fv(chicken3421::get_uniform_location(renderer.program, "dir_light.u_dirlight_ambient"), 1, glm::value_ptr(scene.dir_light.ambient));
    glUniform3fv(chicken3421::get_uniform_location(renderer.program, "dir_light.u_dirlight_diffuse"), 1, glm::value_ptr(scene.dir_light.diffuse));
    glUniform3fv(chicken3421::get_uniform_location(renderer.program, "dir_light.u_dirlight_specular"), 1, glm::value_ptr(scene.dir_light.specular));
    glUniform3fv(chicken3421::get_uniform_location(renderer.program, "dir_light.u_dirlight_pos"), 1, glm::value_ptr(scene.dir_light.position));

    // an iterative approach allows for certain optimisations like tail-recursion not used here.
    // also, recursion of arbitrary depth is impossible, so this approach is more robust.
    std::stack<std::pair<const node_t &, glm::mat4>> renderables{{{ scene.root, scene.root.transform }}};
    while (!renderables.empty()) {
        const auto[node, parent_tf] = renderables.top();  // this is called a structured binding
        renderables.pop();

        glm::mat4 xform = parent_tf * node.transform;

        if (incl_mirror) {
            switch (node.kind) {
                case node_t::MIRROR:
                    draw_mirror(renderer, p, v, xform, node.obj, node.texture);
                    break;
                case node_t::SPHERE:
                    draw_reflective(renderer, p, v, xform, node.obj, (GLuint) 1);
                    break;
                default:
                    draw(renderer, p, v, xform, node.obj);
                    break;
            }
        }
        else {
            if (node.kind != node_t::MIRROR and node.kind != node_t::SPHERE) {
                draw(renderer, p, v, xform, node.obj);
            }
        }

        for (const node_t &c: node.children) {
            renderables.push({ c, xform });
        }
    }
}

void post_process_hdr(const renderer_t &renderer, const model_t &screen_quad) {
    GLuint hdr_program = load_program("res/shaders/hdr.vert", "res/shaders/hdr.frag");
    glUseProgram(hdr_program);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screen_quad.material.diff_map);
    glBindVertexArray(screen_quad.mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, screen_quad.mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_quad.mesh.ibo);

    glDrawElements(GL_TRIANGLES, screen_quad.mesh.nverts, index_type, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void post_process(const renderer_t &renderer, const model_t &screen_quad, int kernel_mode) {
    if (kernel_mode == 0) {
        glUseProgram(renderer.prog_filter);
    }
    else {
        glUseProgram(renderer.prog_kern_filter);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screen_quad.material.diff_map);
    glBindVertexArray(screen_quad.mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, screen_quad.mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_quad.mesh.ibo);

    glDrawElements(GL_TRIANGLES, screen_quad.mesh.nverts, index_type, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

