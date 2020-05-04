#include <imgui/imgui.h>
#include <shader_editor/renderer.h>
#include <shader_editor/ui.h>

void show_menu_bar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Reload Shaders")) {
                reload_shaders();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
