#include <GLFW/glfw3.h>
#include <TextEditor.h>
#include <fstream>
#include <imgui/imgui.h>
#include <shader_editor/renderer.h>
#include <shader_editor/ui.h>

#define DEFAULT_VS_PATH "./test_shader.vs"
#define DEFAULT_FS_PATH "./test_shader.fs"

/*******************/
/* state variables */
/*******************/
static bool b_editor_ready;

static TextEditor editor_fs;
static TextEditor editor_vs;
static std::string editor_vs_path = DEFAULT_VS_PATH;
static std::string editor_fs_path = DEFAULT_FS_PATH;

/****************************/
/* editor related functions */
/****************************/
static void show_editor_menu(TextEditor &);
static void editor_setup(TextEditor &);
static void editor_save_text(TextEditor &, const char *);
static void editor_load_file(TextEditor &, const char *);
static void append_main_menu_bar();

static void show_editor_menu(TextEditor &editor) {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save")) {
                /// save text
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            bool ro = editor.IsReadOnly();
            if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                editor.SetReadOnly(ro);
            ImGui::Separator();

            if (ImGui::MenuItem("Undo", "Ctrl-Z", nullptr,
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
                                !ro && ImGui::GetClipboardText() != nullptr))
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
}

static void editor_setup(TextEditor &editor) {
    auto lang = TextEditor::LanguageDefinition::GLSL();

    // set your own known preprocessor symbols...
    static const char *ppnames[] = {};
    // ... and their corresponding values
    static const char *ppvalues[] = {};

    for (int i = 0; i < sizeof(ppnames) / sizeof(ppnames[0]); ++i) {
        TextEditor::Identifier id;
        id.mDeclaration = ppvalues[i];
        lang.mPreprocIdentifiers.insert(
            std::make_pair(std::string(ppnames[i]), id));
    }

    // set your own identifiers
    static const char *identifiers[] = {};
    static const char *idecls[] = {};

    for (int i = 0; i < sizeof(identifiers) / sizeof(identifiers[0]); ++i) {
        TextEditor::Identifier id;
        id.mDeclaration = std::string(idecls[i]);
        lang.mIdentifiers.insert(
            std::make_pair(std::string(identifiers[i]), id));
    }
    editor.SetLanguageDefinition(lang);
    // editor.SetPalette(TextEditor::GetLightPalette());

    // error markers
    // TextEditor::ErrorMarkers markers;
    // markers.insert(std::make_pair<int, std::string>(
    //     6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
    // markers.insert(
    //     std::make_pair<int, std::string>(41, "Another example error"));
    // editor.SetErrorMarkers(markers);

    // "breakpoint" markers
    // TextEditor::Breakpoints bpts;
    // bpts.insert(24);
    // bpts.insert(47);
    // editor.SetBreakpoints(bpts);
}

static void editor_save_text(TextEditor &editor, const char *file_path) {
    auto file = std::ofstream(file_path);
    if (file.good()) {
        file << editor.GetText();
        file.close();
    }
}

static void editor_load_file(TextEditor &editor, const char *file_path) {
    std::ifstream t(file_path);
    if (t.good()) {
        std::string str((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        editor.SetText(str);
    }
    t.close();
}

void show_glsl_editor_window() {
    if (!b_editor_ready) {
        editor_setup(editor_vs);
        editor_setup(editor_fs);
        editor_load_file(editor_vs, editor_vs_path.c_str());
        editor_load_file(editor_fs, editor_fs_path.c_str());
        b_editor_ready = true;
    }

    append_main_menu_bar();

    auto flags = 0;
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::Begin("Vertex Shader", nullptr, flags);
    {
        auto &editor = editor_vs;
        auto cpos = editor.GetCursorPosition();
        ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1,
                    cpos.mColumn + 1, editor.GetTotalLines(),
                    editor.IsOverwrite() ? "Ovr" : "Ins",
                    editor.CanUndo() ? "*" : " ",
                    editor.GetLanguageDefinition().mName.c_str(),
                    editor_vs_path.c_str());

        // ImGui::SetWindowFontScale(1.2f);
        editor.Render("vs");
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::Begin("Fragment Shader", nullptr, flags);
    {
        auto &editor = editor_fs;
        auto cpos = editor.GetCursorPosition();
        ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1,
                    cpos.mColumn + 1, editor.GetTotalLines(),
                    editor.IsOverwrite() ? "Ovr" : "Ins",
                    editor.CanUndo() ? "*" : " ",
                    editor.GetLanguageDefinition().mName.c_str(),
                    editor_fs_path.c_str());

        // ImGui::SetWindowFontScale(1.2f);
        editor.Render("fs");
    }
    ImGui::End();
}

static void append_main_menu_bar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save Shaders")) {
                editor_save_text(editor_vs, editor_vs_path.c_str());
                editor_save_text(editor_fs, editor_fs_path.c_str());
            }
            if (ImGui::MenuItem("Save and Re-Compile Shaders", "Ctrl+F5")) {
                editor_save_text(editor_vs, editor_vs_path.c_str());
                editor_save_text(editor_fs, editor_fs_path.c_str());
                load_shaders(editor_vs_path.c_str(), editor_fs_path.c_str());
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    bool key_down = true;
    key_down &= (ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) ||
                 ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL));
    key_down &= ImGui::IsKeyPressed(GLFW_KEY_F5);

    if (key_down) {
        editor_save_text(editor_vs, editor_vs_path.c_str());
        editor_save_text(editor_fs, editor_fs_path.c_str());
        load_shaders(editor_vs_path.c_str(), editor_fs_path.c_str());
    }
}
