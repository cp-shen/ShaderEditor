cmake_minimum_required(VERSION 3.10)
project(shader-editor-project)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(3rdparty)

set(UI_SOURCES
    src/ui/glsl_editor_window.cpp
    src/ui/log_window.cpp
    src/ui/render_window.cpp
    src/ui/test_window.cpp
    src/ui/uniform_window.cpp
    src/ui/vertex_input_window.cpp
    src/ui/menu_bar.cpp
    )

set(SHADER_EDITOR_SOURCES
    src/main.cpp
    src/renderer.cpp
    src/mesh_feeder.cpp
    src/3rd_impl.cpp
    src/code_editor.cpp
    ${UI_SOURCES}
    )

set(SHADER_EDITOR_HEADERS
    include/shader_editor/common.h
    include/shader_editor/code_editor.h
    include/shader_editor/ui.h
    include/shader_editor/renderer.h
    include/shader_editor/mesh_feeder.h
    )

add_executable(shader_editor ${SHADER_EDITOR_SOURCES} ${SHADER_EDITOR_HEADERS})

target_include_directories(shader_editor PRIVATE include)
target_include_directories(shader_editor PRIVATE 3rdparty)

target_link_libraries(shader_editor PRIVATE imgui_glfw_opengl3)
target_link_libraries(shader_editor PRIVATE stb)
target_link_libraries(shader_editor PRIVATE ImGuiColorTextEdit)
target_link_libraries(shader_editor PRIVATE LearnOpenGL)
