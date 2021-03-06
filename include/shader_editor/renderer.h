/********************************/
/* Date: 2020-05-03             */
/* Desc: rendering related code */
/********************************/

#ifndef RENDERER_H
#define RENDERER_H

#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#define CLEAR_COLOR 0.1f, 0.1f, 0.1f, 1.0f

#define DEFAULT_VS_PATH "./test_shader.vs"
#define DEFAULT_FS_PATH "./test_shader.fs"
#define DEFAULT_RENDER_OUT_PATH "./out.png"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <learnopengl/camera.h>
#include <string>
#include <unordered_map>
#include <variant>

typedef std::variant<bool, int, float, glm::vec2, glm::vec3, glm::vec4,
                     glm::mat2, glm::mat3, glm::mat4>
    uniform_value_t;

template <class T> struct always_false : std::false_type {};

struct uniform_t {
    std::string name;
    uniform_value_t value;
    void submit();
    uniform_t(const char *n, uniform_value_t v) : name(n), value(v) {}
};

void load_shaders(const char *vs, const char *fs);
void save_texture(const char *path);
void do_offscreen_rendering();
void add_or_set_uniform(const char *, uniform_value_t v);
std::unordered_map<std::string, uniform_t> &get_uniforms();
unsigned get_render_texture_id();
Camera &get_camera();
void renderer_init();
void reset_camara();

#endif
