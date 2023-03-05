#include <Disaster/System/Color.hpp>

namespace px::disaster {
  Color::Color() : r(0), g(0), b(0), a(0) {}
  Color::Color(ColorComponent r, ColorComponent g, ColorComponent b, ColorComponent a)
    : r(r), g(g), b(b), a(a) {}
}