#include <glad/glad.h>

#include <GLFW/glfw3.h> // must after glad is included

#include <imgui/imgui.h>
#include <learnopengl/camera.h>
#include <learnopengl/filesystem.h>
#include <learnopengl/model.h>
#include <learnopengl/shader_m.h>
#include <shader_editor/renderer.h>

#include <stb_image.h>
#include <stb_image_write.h>

static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

struct render_resources_t {
    GLuint fbo;
    GLuint fboColorTex;

    GLuint cubeTexture;
    GLuint floorTexture;

    GLuint cubeVAO;
    GLuint cubeVBO;
    GLuint planeVAO;
    GLuint planeVBO;
    GLuint quadVAO;
    GLuint quadVBO;

    Shader *shader;
    Shader *screenShader;
};

static render_resources_t r;

void feed_render_resources() {
    /*****************************/
    /* build and compile shaders */
    /*****************************/
    r.shader = new Shader("resources/test/framebuffers.vs",
                          "resources/test/framebuffers.fs");

    r.screenShader = new Shader("resources/test/framebuffers_screen.vs",
                                "resources/test/framebuffers_screen.fs");

    /**********************/
    /* set up vertex data */
    /**********************/
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
    float planeVertices[] = {
        // positions          // texture Coords
        5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, 5.0f,
        0.0f, 0.0f,  -5.0f, -0.5f, -5.0f, 0.0f,  2.0f,

        5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, -5.0f,
        0.0f, 2.0f,  5.0f,  -0.5f, -5.0f, 2.0f,  2.0f};
    float quadVertices[] = {// vertex attributes for a quad that fills the
                            // entire screen in Normalized Device Coordinates.
                            // positions   // texCoords
                            -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                            0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                            -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                            1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};

    // cube VAO
    glGenVertexArrays(1, &r.cubeVAO);
    glGenBuffers(1, &r.cubeVBO);
    glBindVertexArray(r.cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, r.cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));

    // plane VAO
    glGenVertexArrays(1, &r.planeVAO);
    glGenBuffers(1, &r.planeVBO);
    glBindVertexArray(r.planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, r.planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));

    // screen quad VAO
    glGenVertexArrays(1, &r.quadVAO);
    glGenBuffers(1, &r.quadVBO);
    glBindVertexArray(r.quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, r.quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));

    /*****************/
    /* load textures */
    /*****************/
    r.cubeTexture = loadTexture(
        FileSystem::getPath("resources/textures/marble.jpg").c_str());
    r.floorTexture = loadTexture(
        FileSystem::getPath("resources/textures/metal.png").c_str());

    /************************/
    /* shader configuration */
    /************************/
    r.shader->use();
    r.shader->setInt("texture1", 0);

    r.screenShader->use();
    r.screenShader->setInt("screenTexture", 0);

    /*****************************/
    /* framebuffer configuration */
    /*****************************/
    glGenFramebuffers(1, &r.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, r.fbo);
    // create a color attachment texture
    glGenTextures(1, &r.fboColorTex);
    glBindTexture(GL_TEXTURE_2D, r.fboColorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           r.fboColorTex, 0);
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

void free_render_resources() {
    glDeleteVertexArrays(1, &r.cubeVAO);
    glDeleteVertexArrays(1, &r.planeVAO);
    glDeleteVertexArrays(1, &r.quadVAO);
    glDeleteBuffers(1, &r.cubeVBO);
    glDeleteBuffers(1, &r.planeVBO);
    glDeleteBuffers(1, &r.quadVBO);

    glDeleteRenderbuffers(1, &r.fbo);

    GLuint textures[] = {r.fboColorTex, r.cubeTexture, r.floorTexture};
    glDeleteTextures(3, textures);

    delete r.shader;
    delete r.screenShader;

    r.shader = r.screenShader = nullptr;
}

GLuint do_offscreen_rendering() {
    /*********************************/
    /* configure global opengl state */
    /*********************************/
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // render
    // ------
    // bind to framebuffer and draw scene as we normally would to color
    // texture
    glBindFramebuffer(GL_FRAMEBUFFER, r.fbo);
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for
                             // rendering screen-space quad)

    // make sure we clear the framebuffer's content
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    r.shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    r.shader->setMat4("view", view);
    r.shader->setMat4("projection", projection);
    // cubes
    glBindVertexArray(r.cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, r.cubeTexture);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    r.shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    r.shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // floor
    glBindVertexArray(r.planeVAO);
    glBindTexture(GL_TEXTURE_2D, r.floorTexture);
    r.shader->setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    static bool saved = false;
    if (!saved) {
        save_texture(SCR_WIDTH, SCR_HEIGHT, r.fboColorTex, "out.png");
        saved = true;
    }

    return r.fboColorTex;
};

void read_pixels_and_save(int w, int h, const char *path) {
    stbi_flip_vertically_on_write(1);
    GLsizei bufSize = w * h * 3;
    GLubyte buf[bufSize];
    glReadnPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, bufSize, buf);
    assert(stbi_write_png(path, w, h, 3, buf, 0) && "failed to write png");
}

void save_texture(int w, int h, GLuint tex, const char *path) {
    stbi_flip_vertically_on_write(1);
    GLsizei bufSize = w * h * 3;
    GLubyte buf[bufSize];
    glGetTextureImage(tex, 0, GL_RGB, GL_UNSIGNED_BYTE, bufSize, buf);
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

void reload_shaders(const char *vs, const char *fs) {
    if (r.shader)
        delete r.shader;
    r.shader = new Shader(vs, fs);
}

/*******************************************************/
/* utility function for loading a 2D texture from file */
/*******************************************************/
unsigned int loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
