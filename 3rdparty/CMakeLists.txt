# glad part
# ---------
add_library(GLAD STATIC "glad/glad.c")
target_include_directories(GLAD PUBLIC .)
target_link_libraries(GLAD PUBLIC ${CMAKE_DL_LIBS})

# imgui part
# ----------

set(IMGUI_SOURCES
    imgui/imconfig.h
    imgui/imgui.cpp
    imgui/imgui.h
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_internal.h
    imgui/imgui_widgets.cpp
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/examples/imgui_impl_glfw.h
    imgui/examples/imgui_impl_glfw.cpp
    imgui/examples/imgui_impl_opengl3.h
    imgui/examples/imgui_impl_opengl3.cpp
    )

add_library(imgui_glfw_opengl3 STATIC ${IMGUI_SOURCES})
target_include_directories(imgui_glfw_opengl3 PUBLIC imgui)
target_compile_definitions(imgui_glfw_opengl3 PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(imgui_glfw_opengl3 PUBLIC glfw)

# find_package(GLEW REQUIRED)
# target_include_directories(imgui_glfw_opengl3 PUBLIC ${GLEW_INCLUDE_DIR})
# target_link_libraries(imgui_glfw_opengl3 PUBLIC GLEW::glew)

target_link_libraries(imgui_glfw_opengl3 PUBLIC GLAD)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
target_link_libraries(imgui_glfw_opengl3 PUBLIC OpenGL::GL)

# ImGuiColorTextEdit
# ------------------

add_library(ImGuiColorTextEdit STATIC
    ImGuiColorTextEdit/TextEditor.h
    ImGuiColorTextEdit/TextEditor.cpp
    )

target_include_directories(ImGuiColorTextEdit PUBLIC imgui)
target_include_directories(ImGuiColorTextEdit PUBLIC ImGuiColorTextEdit)

# stb part
# --------

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb)

# learnopengl part
# ----------------

add_subdirectory(LearnOpenGL)
add_library(LearnOpenGL INTERFACE)
target_include_directories(LearnOpenGL INTERFACE LearnOpenGL/includes)
target_include_directories(LearnOpenGL INTERFACE LearnOpenGL/configuration)
