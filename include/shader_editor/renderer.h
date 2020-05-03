/********************************/
/* Date: 2020-05-03             */
/* Desc: rendering related code */
/********************************/

#ifndef RENDERER_H
#define RENDERER_H

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

void free_render_resources();
void feed_render_resources();
void reload_shaders();
unsigned int do_offscreen_rendering();
unsigned int loadTexture(const char *path);
void read_pixels_and_save(int w, int h, const char *path);
void save_texture(int w, int h, unsigned tex, const char *path);
void process_camara_input();

#endif
