#include <imgui/imgui.h>
#include <shader_editor/renderer.h>
#include <shader_editor/ui.h>

#include <GLFW/glfw3.h>

void show_menu_bar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Quit")) {
                auto *w = glfwGetCurrentContext();
                if (w)
                    glfwSetWindowShouldClose(w, true);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Reset Camara")) {
                reset_camara();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
