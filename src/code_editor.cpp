#include <fstream>
#include <streambuf>

#include <shader_editor/code_editor.h>
#include <shader_editor/common.h>

void editor_setup(TextEditor &editor) {
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

void editor_save_text(TextEditor &editor, const char *file_path) {
    auto file = std::ofstream(file_path);
    if (file.good()) {
        file << editor.GetText();
        file.close();
    }
}

void editor_load_file(TextEditor &editor, const char *file_path) {
    std::ifstream t(file_path);
    if (t.good()) {
        std::string str((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        editor.SetText(str);
    }
    t.close();
}

