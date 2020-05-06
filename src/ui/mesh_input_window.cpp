#include <imgui/imgui.h>
#include <numeric>
#include <shader_editor/mesh_feeder.h>
#include <shader_editor/ui.h>

void show_mesh_input_window() {
    ImGui::Begin("Mesh Input");

    auto &mesh_loaded = get_mesh_loaded();
    ImGui::Text("Mesh Count: %d", (int)mesh_loaded.size());

    int v_count = std::accumulate(
        mesh_loaded.begin(), mesh_loaded.end(), 0,
        [](int count, Mesh &m) { return m.vertices.size() + count; });

    ImGui::Text("Vertex Count: %d", v_count);

    ImGui::End();
}
