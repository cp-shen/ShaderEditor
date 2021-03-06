#include <iostream>
#include <stdio.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

#include <shader_editor/common.h>
#include <shader_editor/mesh_feeder.h>
#include <shader_editor/renderer.h>
#include <shader_editor/ui.h>

static GLFWwindow *window;

static void process_app_main_loop();
static void process_ui_main_loop();
static int app_init();
static void app_destroy();

static void error_callback(int error, const char *description) {
    fprintf(stderr, "Error %d: %s\n", error, description);
}

static void process_ui_main_loop() {
    show_menu_bar();
    show_render_window();
    // show_test_windows();
    show_log_window();
    show_glsl_editor_window();
    show_uniform_window();
    show_mesh_input_window();
}

static void process_modules_main_loop() {
    // render to texture
    do_offscreen_rendering();
}

static void modules_init() {
    LOG_MSG("modules init begin...");

    mesh_feeder_init();
    renderer_init();

    LOG_MSG("modules init success");
}
static void modules_destroy() { save_texture(DEFAULT_RENDER_OUT_PATH); }

static void process_app_main_loop() {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // update modules
    process_modules_main_loop();
    // update ui
    process_ui_main_loop();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, display_w, display_h);
    glClearColor(CLEAR_COLOR);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
}

static int app_init() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;

        // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
    // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

    // Create window with graphics context
    window = glfwCreateWindow(1280, 720, "Shader Editor", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    bool err = gladLoadGL() == 0;
    if (err) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable
    // Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable
    // Multi-Viewport

    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform
    // windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // ...

    // init success
    return 0;
}

static void app_destroy() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

int main(int, char **) {
    assert(app_init() == 0 && "app init failed");
    modules_init();

    while (!glfwWindowShouldClose(window)) {
        process_app_main_loop();
    }

    modules_destroy();
    app_destroy();
    LOG_MSG("app exit normally");
    return 0;
}
