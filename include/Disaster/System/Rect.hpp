#ifndef PX_DISASTER_SYSTEM_RECT_HPP
#define PX_DISASTER_SYSTEM_RECT_HPP
#include <Disaster/System/Vector2.hpp>

namespace px::disaster {
  template<class T>
  struct Rect {
  public:
    Rect();
    Rect(Vector2<T> leftTop, Vector2<T> size);
    Rect(T left, T top, T width, T height);

    bool Contains(Vector2<T> point) const;
    bool Contains(T x, T y) const;
    bool Intersects(Rect<T> anotherRect) const;

    template<class U>
    Rect<U> Convert() const;

    T left;
    T top;
    T width;
    T height;
  };

  typedef Rect<int> IntRect;
  typedef Rect<unsigned int> UintRect;
  typedef Rect<float> FloatRect;

  template<class T>
  Rect<T>::Rect() : left(0), top(0), width(0), height(0) {}

  template<class T>
  Rect<T>::Rect(Vector2<T> lt, Vector2<T> size) 
    : left(lt.left), top(lt.top), width(size.width), height(size.height) {}

  template<class T>
  Rect<T>::Rect(T left, T top, T width, T height) 
    : left(left), top(top), width(width), height(height) {}

  template<class T>
  bool Rect<T>::Contains(Vector2<T> point) const {
    return Contains(point.x, point.y);
  }
  template<class T>
  bool Rect<T>::Contains(T x, T y) const {
    return (x > left && x < left + width) && (y > top && y < top + width); 
  }
  template<class T>
  bool Rect<T>::Intersects(Rect<T> anotherRect) const {
    T right = left + width;
    T bottom = top + height;

    T otherRight = anotherRect.left + anotherRect.width;
    T otherBottom = anotherRect.top + anotherRect.height;

    if (left >= otherRight || right <= anotherRect.left ||
        top >= otherBottom || bottom <= anotherRect.top) {
      return false;
    }
    return true;
  }

  template<class T>
  template<class U>
  Rect<U> Rect<T>::Convert() const {
    return Rect<U>((U) left, (U) top, (U) width, (U) height);
  }
}

#endif // PX_DISASTER_SYSTEM_RECT_HPP