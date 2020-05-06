#include <shader_editor/ui.h>
#include <shader_editor/common.h>

#define LOG_WINDOW_TITLE "Renderer Log"

static ExampleAppLog logger;

void show_log_window(bool *p_open) {
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

    ImGui::Begin(LOG_WINDOW_TITLE, p_open);
    // widgets on ytp of logs
    ImGui::End();

    logger.Draw(LOG_WINDOW_TITLE, p_open);
}

void show_log_window() { show_log_window(nullptr); }


ExampleAppLog &get_logger(){
    return logger;
}
