/********************************/
/* Date: 2020-05-03             */
/* Desc: rendering related code */
/********************************/

#ifndef RENDERER_H
#define RENDERER_H

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#include <glm/glm.hpp>
#include <string>
#include <variant>

typedef std::variant<bool, int, float, glm::vec2, glm::vec3, glm::vec4,
                     glm::mat2, glm::mat3, glm::mat4>
    uniform_value_t;

template <class T> struct always_false : std::false_type {};

struct uniform_t {
    std::string name;
    uniform_value_t value;
    void submit();
};

void load_shaders(const char *vs, const char *fs);
void load_model(const char *);
void save_texture(const char *path);
void process_camara_input();
unsigned int do_offscreen_rendering();
void add_uniform(uniform_t u);

#endif
