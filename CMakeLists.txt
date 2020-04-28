cmake_minimum_required(VERSION 3.10)
project(shader-editor-project)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(libs)

set(SHADER_EDITOR_SOURCES
    src/main.cpp
    src/framebuffers.cpp
    )
add_executable(shader_editor ${SHADER_EDITOR_SOURCES})

target_include_directories(shader_editor PRIVATE include)
target_include_directories(shader_editor PRIVATE libs)
target_link_libraries(shader_editor PRIVATE imgui_glfw_opengl3)
target_link_libraries(shader_editor PRIVATE stb)
