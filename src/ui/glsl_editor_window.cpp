#include <imgui/imgui.h>
#include <shader_editor/code_editor.h>
#include <shader_editor/ui.h>

/*******************/
/* state variables */
/*******************/
static TextEditor editor;
static std::string editor_file_path;
static bool editor_ready;
static bool b_show_editor_window = true;

void show_glsl_editor_window() {
    if (!editor_ready) {
        editor_setup(editor);
        editor_ready = true;
    }

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Text Editor Demo", &b_show_editor_window,
                 ImGuiWindowFlags_HorizontalScrollbar |
                     ImGuiWindowFlags_MenuBar);
    {
        auto cpos = editor.GetCursorPosition();
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save")) {
                    /// save text....
                    // FIXME
                    editor_save_text(editor, editor_file_path.c_str());
                }
                if (ImGui::MenuItem("Quit", "Alt-F4")) {
                    /// set close window flag
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                bool ro = editor.IsReadOnly();
                if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                    editor.SetReadOnly(ro);
                ImGui::Separator();

                if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr,
                                    !ro && editor.CanUndo()))
                    editor.Undo();
                if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr,
                                    !ro && editor.CanRedo()))
                    editor.Redo();

                ImGui::Separator();

                if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr,
                                    editor.HasSelection()))
                    editor.Copy();
                if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr,
                                    !ro && editor.HasSelection()))
                    editor.Cut();
                if (ImGui::MenuItem("Delete", "Del", nullptr,
                                    !ro && editor.HasSelection()))
                    editor.Delete();
                if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr,
                                    !ro &&
                                        ImGui::GetClipboardText() != nullptr))
                    editor.Paste();

                ImGui::Separator();

                if (ImGui::MenuItem("Select all", nullptr, nullptr))
                    editor.SetSelection(
                        TextEditor::Coordinates(),
                        TextEditor::Coordinates(editor.GetTotalLines(), 0));

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("Dark palette"))
                    editor.SetPalette(TextEditor::GetDarkPalette());
                if (ImGui::MenuItem("Light palette"))
                    editor.SetPalette(TextEditor::GetLightPalette());
                if (ImGui::MenuItem("Retro blue palette"))
                    editor.SetPalette(TextEditor::GetRetroBluePalette());
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Text(
            "%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1,
            cpos.mColumn + 1, editor.GetTotalLines(),
            editor.IsOverwrite() ? "Ovr" : "Ins", editor.CanUndo() ? "*" : " ",
            editor.GetLanguageDefinition().mName.c_str(), "filename here");

        editor.Render("TextEditor");
    }
    ImGui::End();
}
