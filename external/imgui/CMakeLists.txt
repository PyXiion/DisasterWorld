cmake_minimum_required(VERSION 3.15)
project(ImGui-SFML)

add_library(imgui STATIC ./imgui.cpp ./imgui_draw.cpp ./imgui_tables.cpp ./imgui_widgets.cpp ./imgui_impl_opengl3.cpp ./imgui_impl_sdl.cpp)

target_link_libraries(imgui GL)
