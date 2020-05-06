#include <imgui/imgui.h>
#include <shader_editor/renderer.h>
#include <shader_editor/ui.h>
#include <shader_editor/common.h>

#include <GLFW/glfw3.h> // must after glad is included

static void process_camara_input() {
    auto &io = ImGui::GetIO();
    auto &camera = get_camera();
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

void show_render_window() {

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Rendering");
    {
        process_camara_input();

        // OpenGL offscreen rendering
        // auto texId = do_offscreen_rendering();
        auto texId = get_render_texture_id();

        // Get the current cursor position (where your window is)
        ImVec2 pos = ImGui::GetCursorScreenPos();

        // ImGui::Image((void *)texId, ImVec2(SCR_WIDTH, SCR_HEIGHT),
        //              ImVec2(0, 0), ImVec2(1, 1));

        ImGui::GetWindowDrawList()->AddImage(
            (void *)texId, pos, ImVec2(pos.x + SCR_WIDTH, pos.y + SCR_HEIGHT),
            ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
}
