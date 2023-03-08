cmake_minimum_required(VERSION 3.15)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_SOURCE_DIR}/cmake/find-modules")

project(DisasterWorld)

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

find_package(yaml-cpp REQUIRED)
find_package(Angelscript REQUIRED)
find_package(easy_profiler REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

include_directories(./include)
include_directories(./external)
include_directories(${EASY_PROFILER_INCLUDE_DIR} ${SDL2_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 23)
set(SRC
  src/main.cpp
  src/Program.cpp
  src/AppConsole.cpp

  src/System/Window.cpp
  src/System/Color.cpp
  
  src/Graphics/VertexArray.cpp
  src/Graphics/Shader.cpp
  src/Graphics/RenderTexture.cpp
  src/Graphics/Texture.cpp
  src/Graphics/Transform.cpp
  src/Graphics/Transformable.cpp
  src/Graphics/Sprite.cpp

  src/Gameplay/Game.cpp
  src/Gameplay/World.cpp
  src/Gameplay/Chunk.cpp
  src/Gameplay/Camera.cpp
  src/Gameplay/Tilemap.cpp
  src/Gameplay/BaseWorldGenerator.cpp

  src/Script/ScriptEngine.cpp
  src/Script/Bindings.cpp

  src/Utils/MemoryStream.cpp
  src/Utils/RamUsage.cpp
  src/Utils/ResourceManager.cpp

  include/Disaster/System/Vector2.hpp
)

add_executable(DisasterWorld ${SRC})

add_subdirectory(./external/imgui)
add_subdirectory(./external/glad)
add_subdirectory(./external/angelscript-addons)

target_compile_definitions(DisasterWorld PUBLIC USING_EASY_PROFILER)

target_link_libraries(DisasterWorld ${SDL2_LIBRARIES} GL glad ${GLM_LIBRARIES})

target_link_libraries(DisasterWorld yaml-cpp easy_profiler imgui)
target_link_libraries(DisasterWorld angelscript angelscript-addons)

include(CPackConfig.cmake)