#ifndef COMMON_H
#define COMMON_H

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#include "learnopengl/camera.h"
#include <GLFW/glfw3.h>
#include <TextEditor.h>

void free_render_resources();
void feed_render_resources();
void editor_setup(TextEditor &);
void process_window_main_loop(GLFWwindow *);
void process_editor_main_loop(TextEditor &);
GLuint do_offscreen_rendering(Camera &camera);
unsigned int loadTexture(const char *path);
void read_pixels_and_save(int w, int h, const char *path);
void save_texture(int w, int h, unsigned tex, const char *path);
void process_camara_input(Camera &camera);

#endif
