#include <imgui/imgui.h>
#include <shader_editor/renderer.h>
#include <shader_editor/ui.h>

static void show_uniform_mat4(uniform_t &uniform) {
    auto &mat4 = std::get<glm::mat4>(uniform.value);

    auto &col0 = mat4[0];
    auto &col1 = mat4[1];
    auto &col2 = mat4[2];
    auto &col3 = mat4[3];

    ImGui::InputFloat4("col0", &col0[0]);
    ImGui::InputFloat4("col1", &col1[0]);
    ImGui::InputFloat4("col2", &col2[0]);
    ImGui::InputFloat4("col3", &col3[0]);
}

static void show_uniform_vec4(uniform_t &uniform) {
    auto &vec4 = std::get<glm::vec4>(uniform.value);
    ImGui::InputFloat4("vec4", &vec4[0]);
}

static void show_uniform(uniform_t &uniform) {
    if (ImGui::TreeNode(uniform.name.c_str())) {
        std::visit(
            [&uniform](auto &&v) {
                using T = std::decay_t<decltype(v)>;
                // TODO: impl all types
                if constexpr (std::is_same_v<T, int>)
                    ;
                else if constexpr (std::is_same_v<T, bool>)
                    ;
                else if constexpr (std::is_same_v<T, int>)
                    ;
                else if constexpr (std::is_same_v<T, float>)
                    ;
                else if constexpr (std::is_same_v<T, glm::vec2>)
                    ;
                else if constexpr (std::is_same_v<T, glm::vec3>)
                    ;
                else if constexpr (std::is_same_v<T, glm::vec4>)
                    show_uniform_vec4(uniform);
                else if constexpr (std::is_same_v<T, glm::mat2>)
                    ;
                else if constexpr (std::is_same_v<T, glm::mat3>)
                    ;
                else if constexpr (std::is_same_v<T, glm::mat4>)
                    show_uniform_mat4(uniform);
                else
                    static_assert(always_false<T>::value,
                                  "non-exhaustive visitor!");
            },
            uniform.value);

        ImGui::TreePop();
    }
}

void show_uniform_window() {
    ImGui::Begin("Uniforms");

    auto &uniforms = get_uniforms();
    ImGui::Text("Uniform Count: %d", (int)uniforms.size());

    for (auto &p : uniforms) {
        show_uniform(p.second);
    }

    ImGui::End();
}
