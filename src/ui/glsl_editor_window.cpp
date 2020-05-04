#include <TextEditor.h>
#include <imgui/imgui.h>
#include <shader_editor/ui.h>

#include <fstream>

#define DEFAULT_VS_PATH "./test_shader.vs"
#define DEFAULT_FS_PATH "./test_shader.fs"

/*******************/
/* state variables */
/*******************/
static bool b_show_editor_window = true;
static bool b_show_vs_code = true;
static bool b_editor_ready;

static TextEditor editor;
static std::string editor_vs_path = DEFAULT_VS_PATH;
static std::string editor_fs_path = DEFAULT_FS_PATH;

static std::string vs_text;
static std::string fs_text;

/****************************/
/* editor related functions */
/****************************/
static void show_editor_menu();
static void editor_setup();
static void editor_save_text(const char *);
static void editor_load_file(const char *);

static void show_editor_menu() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save")) {
                editor_save_text(editor_fs_path.c_str());
            }
            if (ImGui::MenuItem("Close")) {
                b_show_editor_window = false;
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

static void editor_setup() {
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

static void editor_save_text(const char *file_path) {
    auto file = std::ofstream(file_path);
    if (file.good()) {
        file << editor.GetText();
        file.close();
    }
}

static void editor_load_file(const char *file_path) {
    std::ifstream t(file_path);
    if (t.good()) {
        std::string str((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        editor.SetText(str);
    }
    t.close();
}

void show_glsl_editor_window() {
    if (!b_show_editor_window)
        return;

    if (!b_editor_ready) {
        editor_setup();
        b_editor_ready = true;
    }

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(800, 600), ImGuiCond_FirstUseEver);

    auto flags =
        ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar;

    if (ImGui::Begin("Text Editor Demo", &b_show_editor_window, flags)) {
        show_editor_menu();

        auto cpos = editor.GetCursorPosition();
        ImGui::Text(
            "%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1,
            cpos.mColumn + 1, editor.GetTotalLines(),
            editor.IsOverwrite() ? "Ovr" : "Ins", editor.CanUndo() ? "*" : " ",
            editor.GetLanguageDefinition().mName.c_str(), "filename here");

        editor.Render("TextEditor");
        ImGui::End();
    }
}
