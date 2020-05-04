#include <imgui/imgui.h>
#include <shader_editor/ui.h>
#include <shader_editor/renderer.h>

void show_render_window() {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Game rendering");
    {
        // OpenGL offscreen rendering
        process_camara_input();
        auto texId = do_offscreen_rendering();

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
