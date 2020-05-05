#include <glad/glad.h>

#include <GLFW/glfw3.h> // must after glad is included

#include <imgui/imgui.h>
#include <learnopengl/camera.h>
#include <learnopengl/filesystem.h>
#include <learnopengl/mesh.h>
#include <learnopengl/model.h>
#include <learnopengl/shader_m.h>
#include <shader_editor/renderer.h>

#include <stb_image.h>
#include <stb_image_write.h>

#include <memory>
#include <unordered_map>
#include <vector>

/*********************************/
/* renderer states and resources */
/*********************************/
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static std::unique_ptr<Shader> shader;
static std::vector<Mesh> mesh_loaded;
static unsigned fbo;
static unsigned fbo_color_tex;
static std::unordered_map<std::string, uniform_t> uniforms;

/****************************/
/* static utility functions */
/****************************/
// static unsigned int loadTexture(char const *path) {
//     unsigned int textureID;
//     glGenTextures(1, &textureID);
//
//     int width, height, nrComponents;
//     unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
//     if (data) {
//         GLenum format;
//         if (nrComponents == 1)
//             format = GL_RED;
//         else if (nrComponents == 3)
//             format = GL_RGB;
//         else if (nrComponents == 4)
//             format = GL_RGBA;
//
//         glBindTexture(GL_TEXTURE_2D, textureID);
//         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
//                      GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);
//
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
//                         GL_LINEAR_MIPMAP_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//         stbi_image_free(data);
//     } else {
//         std::cout << "Texture failed to load at path: " << path << std::endl;
//         stbi_image_free(data);
//     }
//
//     return textureID;
// }

static void submit_uniforms() {
    shader->use();
    for (auto &pair : uniforms) {
        pair.second.submit();
    }
}

static void add_default_uniforms() {
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    add_or_set_uniform("projection", projection);

    glm::mat4 view = camera.GetViewMatrix();
    add_or_set_uniform("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    // it's a bit too big for our scene, so scale it down
    add_or_set_uniform("model", model);
}

static void renderer_setup_framebuffer() {
    /*****************************/
    /* framebuffer configuration */
    /*****************************/
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // create a color attachment texture
    glGenTextures(1, &fbo_color_tex);
    glBindTexture(GL_TEXTURE_2D, fbo_color_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           fbo_color_tex, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't
    // be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH,
                          SCR_HEIGHT); // use a single renderbuffer object for
                                       // both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, rbo); // now actually attach it

    // now that we actually created the framebuffer and added all attachments we
    // want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void draw() {
    shader->use();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    for (unsigned i = 0; i < mesh_loaded.size(); i++)
        mesh_loaded[i].Draw(*shader);

    // make sure we clear the framebuffer's content
    glClearColor(CLEAR_COLOR);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*****************/
/* api functions */
/*****************/

unsigned do_offscreen_rendering() {
    static bool setup = false;
    if (!setup) {
        renderer_setup_framebuffer();
        add_default_uniforms();
        setup = true;
    }

    if (shader != nullptr || mesh_loaded.empty())
        return 0; // FIXME

    // assert(shader != nullptr);
    // assert(mesh_loaded.empty());

    submit_uniforms(); // FIXME: optimize this
    draw();

    return fbo_color_tex;
};

// void read_pixels_and_save(int w, int h, const char *path) {
//     stbi_flip_vertically_on_write(1);
//     GLsizei bufSize = w * h * 3;
//     GLubyte buf[bufSize];
//     glReadnPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, bufSize, buf);
//     assert(stbi_write_png(path, w, h, 3, buf, 0) && "failed to write png");
// }

void save_texture(const char *path) {
    stbi_flip_vertically_on_write(1);

    unsigned w = SCR_WIDTH;
    unsigned h = SCR_HEIGHT;
    GLsizei bufSize = w * h * 3;

    GLubyte buf[bufSize];
    glGetTextureImage(fbo_color_tex, 0, GL_RGB, GL_UNSIGNED_BYTE, bufSize, buf);
    assert(stbi_write_png(path, w, h, 3, buf, 0) && "failed to write png");
}

void process_camara_input() {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL &&
              "Missing dear imgui context.");

    auto &io = ImGui::GetIO();
    auto *window = (GLFWwindow *)ImGui::GetWindowViewport()->PlatformHandle;

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow)) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    }

    /*****************/
    /* keybord input */
    /*****************/
    auto deltaTime = io.DeltaTime;
    if (ImGui::IsKeyDown(GLFW_KEY_W))
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (ImGui::IsKeyDown(GLFW_KEY_S))
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (ImGui::IsKeyDown(GLFW_KEY_A))
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (ImGui::IsKeyDown(GLFW_KEY_D))
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (ImGui::IsKeyDown(GLFW_KEY_ESCAPE))
        ImGui::SetNextWindowFocus();

    /******************/
    /* mouse movement */
    /******************/
    camera.ProcessMouseMovement(io.MouseDelta.x, -io.MouseDelta.y);
}

void load_shaders(const char *vs, const char *fs) {
    shader.reset(new Shader(vs, fs));
}

void load_model(const char *file_path) {
    mesh_loaded = std::move(Model(file_path).meshes);
}

void uniform_t::submit() {
    std::visit(
        [this](auto &&v) {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, int>)
                shader->setInt(this->name, v);
            else if constexpr (std::is_same_v<T, bool>)
                shader->setBool(this->name, v);
            else if constexpr (std::is_same_v<T, int>)
                shader->setInt(this->name, v);
            else if constexpr (std::is_same_v<T, float>)
                shader->setFloat(this->name, v);
            else if constexpr (std::is_same_v<T, glm::vec2>)
                shader->setVec2(this->name, v);
            else if constexpr (std::is_same_v<T, glm::vec3>)
                shader->setVec3(this->name, v);
            else if constexpr (std::is_same_v<T, glm::vec4>)
                shader->setVec4(this->name, v);
            else if constexpr (std::is_same_v<T, glm::mat2>)
                shader->setMat2(this->name, v);
            else if constexpr (std::is_same_v<T, glm::mat3>)
                shader->setMat3(this->name, v);
            else if constexpr (std::is_same_v<T, glm::mat4>)
                shader->setMat4(this->name, v);
            else
                static_assert(always_false<T>::value,
                              "non-exhaustive visitor!");
        },
        this->value);
}

void set_uniform_value(const char *n, uniform_value_t v) {
    auto &u = uniforms.at(n);
}

void add_or_set_uniform(const char *n, uniform_value_t v) {
    if (!uniforms.count(n))
        uniforms.emplace(std::make_pair(n, uniform_t(n, v)));
    else
        uniforms.at(n).value = v;
}

std::unordered_map<std::string, uniform_t> &get_uniforms() { return uniforms; }
