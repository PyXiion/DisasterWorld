#ifndef PX_DISASTER_SYSTEM_RECT_HPP
#define PX_DISASTER_SYSTEM_RECT_HPP

namespace px::disaster {
  template<class T>
  struct Rect {
  public:
    Rect();
    Rect(T left, T top, T width, T height);

    bool Contains(Vector2<T> point) const;
    bool Contains(T x, T y) const;
    bool Intersects(Rect<T> anotherRect) const;

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
  bool Rect<T>::Intersects(Rect<T> rect) const {
    return Contains(rect.left, rect.top) || Contains(rect.left + width, rect.top) ||
           Contains(rect.left, rect.top + height)  || Contains(rect.left + width, rect.top + height);
  }
}

#endif // PX_DISASTER_SYSTEM_RECT_HPP