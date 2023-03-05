cmake_minimum_required(VERSION 3.15)
project(AngelScript_Addons)

add_library(angelscript-addons 
  ./add_on/scriptstdstring/scriptstdstring.cpp 
  ./add_on/scriptstdstring/scriptstdstring_utils.cpp
  
  ./add_on/scripthelper/scripthelper.cpp)

target_link_libraries(angelscript-addons angelscript)