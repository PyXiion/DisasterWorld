#include <iostream>

#include <Disaster/Program.hpp>
#include <easy/profiler.h>

int main(int argc, char** argv) {
  EASY_PROFILER_ENABLE;
  EASY_MAIN_THREAD;
  // try {
    px::disaster::Program::GetInstance().Main();
  // } catch (const std::exception &e) {
    // std::cout << "An error has occurred: " << e.what() << std::endl;
  // }
  profiler::dumpBlocksToFile("debug.prof");
  return 0;
}