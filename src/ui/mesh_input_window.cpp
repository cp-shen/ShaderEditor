#include <imgui/imgui.h>
#include <shader_editor/mesh_feeder.h>
#include <shader_editor/ui.h>

void show_mesh_input_window() {
    ImGui::Begin("Mesh Input");

    auto &mesh_loaded = get_mesh_loaded();
    ImGui::Text("Mesh Count: %d", (int)mesh_loaded.size());

    ImGui::End();
}
