#ifndef PX_DISASTER_SYSTEM_COLOR_HPP
#define PX_DISASTER_SYSTEM_COLOR_HPP
#include <cstdint>

namespace px::disaster {
  typedef uint8_t ColorComponent;
  
  struct Color {
  public:
    Color();
    Color(ColorComponent r = 255, ColorComponent g = 255, ColorComponent b = 255, ColorComponent a = 255);

    ColorComponent r;
    ColorComponent g;
    ColorComponent b;
    ColorComponent a;
  };
}

#endif // PX_DISASTER_SYSTEM_COLOR_HPP