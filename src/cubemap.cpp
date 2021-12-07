#include <glad/glad.h>

#include <chicken3421/chicken3421.hpp>

#include <ass3/cubemap.hpp>
#include <iostream>

GLuint make_cubemap(const std::string &base_path, const std::string &extension) {
    static const char *suffixes[] = { "right", "left", "top", "bottom", "front", "back" };
    
    GLuint cubemap = chicken3421::make_texture();
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

    for (int side = 0; side < 6; side++) {
        std::string path = base_path;
        path += suffixes[side];
        path += extension; // a very C++ sort of thing
        chicken3421::image_t image = chicken3421::load_image(path, false);
        GLenum fmt = image.n_channels == 3 ? GL_RGB : GL_RGBA;

        GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + side;
        glTexImage2D(face, 0, fmt, image.width, image.height, 0, fmt, GL_UNSIGNED_BYTE, image.data);
        chicken3421::delete_image(image);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return cubemap;
}
