/************************************/
/* Date Created: 2020-04-30         */
/* Desc: top level window functions */
/************************************/

#include <imgui/imgui.h>
#include <shader_editor/common.h>
#include <shader_editor/renderer.h>
#include <shader_editor/ui.h>

// Our state
static bool b_show_demo_window = true;
static bool b_show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void process_ui_main_loop() {}

void show_demo_window() {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    if (b_show_demo_window)
        ImGui::ShowDemoWindow(&b_show_demo_window);
}

void show_hw_window() {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Hello, world!");
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Text("This is some useful text."); // Display some text (you can
                                                  // use a format strings too)
        ImGui::Checkbox("Demo Window",
                        &b_show_demo_window); // Edit bools storing our window
                                              // open/close state
        ImGui::Checkbox("Another Window", &b_show_another_window);

        ImGui::SliderFloat(
            "float", &f, 0.0f,
            1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3(
            "clear color",
            (float *)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button(
                "Button")) // Buttons return true when clicked (most widgets
                           // return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
    }
    ImGui::End();
}

void show_another_window() {
    if (b_show_another_window) {
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
        ImGui::Begin(
            "Another Window",
            &b_show_another_window); // Pass a pointer to our bool variable
                                     // (the window will have a closing button
                                     // that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            b_show_another_window = false;
        ImGui::End();
    }
}

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
