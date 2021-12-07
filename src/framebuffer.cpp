#include <ass3/framebuffer.hpp>

#include <chicken3421/chicken3421.hpp>

framebuffer_t make_framebuffer(int width, int height) {
    framebuffer_t fbuff{};

    // create framebuffer
    fbuff.fbo = chicken3421::make_framebuffer();
    glBindFramebuffer(GL_FRAMEBUFFER, fbuff.fbo);

    // create texture for framebuffer color
    fbuff.texture = chicken3421::make_texture();
    glBindTexture(GL_TEXTURE_2D, fbuff.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // create renderbuffer for framebuffer depth/stencil
    fbuff.rbo = chicken3421::make_renderbuffer();
    glBindRenderbuffer(GL_RENDERBUFFER, fbuff.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    // attach texture and renderbuffer to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbuff.texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbuff.rbo);

    // reset state
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return fbuff;
}

void delete_framebuffer(framebuffer_t &fbuff) {
    chicken3421::delete_framebuffer(fbuff.fbo);
    chicken3421::delete_renderbuffer(fbuff.rbo);
    chicken3421::delete_texture(fbuff.texture);
}

