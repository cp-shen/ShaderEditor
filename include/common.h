#ifndef COMMON_H
#define COMMON_H

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#include <learnopengl/camera.h>

void free_render_resources();
void feed_render_resources();
GLuint do_offscreen_rendering(Camera &camera);
unsigned int loadTexture(const char *path);
void read_pixels_and_save(int w, int h, const char *path);
void save_texture(int w, int h, unsigned tex, const char *path);

#endif
