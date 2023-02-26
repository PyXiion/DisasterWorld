cmake_minimum_required(VERSION 3.15)
project(DisasterWorld)

include_directories(./include)
include_directories(./external)
link_directories(./libs)

set(CMAKE_CXX_STANDARD 23)
set(SRC
  src/main.cpp
  src/Program.cpp
  src/AppConsole.cpp
  
  src/Gameplay/Game.cpp
  src/Gameplay/World.cpp
  src/Gameplay/Chunk.cpp
  src/Gameplay/Camera.cpp
  src/Gameplay/Tilemap.cpp
  src/Gameplay/BaseWorldGenerator.cpp

  src/Script/ScriptEngine.cpp

  src/Utils/MemoryStream.cpp
  src/Utils/TextureManager.cpp
  src/Utils/RamUsage.cpp
)

add_executable(DisasterWorld ${SRC})

add_subdirectory(./external/imgui-sfml)
add_subdirectory(./external/angelscript)

target_compile_definitions(DisasterWorld PUBLIC USING_EASY_PROFILER)

target_link_libraries(DisasterWorld sfml-system sfml-window sfml-graphics sfml-imgui)
target_link_libraries(DisasterWorld yaml-cpp easy_profiler)
target_link_libraries(DisasterWorld angelscript angelscript-addons)

include(CPackConfig.cmake)