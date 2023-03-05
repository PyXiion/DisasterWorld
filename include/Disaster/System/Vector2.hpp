#ifndef PX_DISASTER_SYSTEM_VECTOR2_HPP
#define PX_DISASTER_SYSTEM_VECTOR2_HPP
#include <cmath>

namespace px::disaster {
  template<class T>
  struct Vector2 {
    Vector2();
    Vector2(T x, T y);

    Vector2<T> operator+(const Vector2<T> &r) const;
    Vector2<T> operator-(const Vector2<T> &r) const;

    Vector2<T> operator*(T factor) const;
    Vector2<T> operator/(T factor) const;

    Vector2<T> &operator+=(const Vector2<T> &r);
    Vector2<T> &operator-=(const Vector2<T> &r);

    Vector2<T> &operator*=(T factor);
    Vector2<T> &operator/=(T factor);

    Vector2<T> &operator=(const Vector2<T> &other);

    template<class U>
    Vector2<U> Convert() const;

    T          Length() const;
    Vector2<T> Direction() const;

    union { T x, width,  left; };
    union { T y, height, top ; };
  };

  typedef Vector2<int> Vector2i;
  typedef Vector2<unsigned int> Vector2u;
  typedef Vector2<float> Vector2f;
  typedef Vector2<double> Vector2d;

  template<class T>
  Vector2<T>::Vector2() : x(0), y(0) {}
  template<class T>
  Vector2<T>::Vector2(T x, T y) : x(x), y(y) {}

  template<class T>
  Vector2<T> Vector2<T>::operator+(const Vector2<T> &r) const {
    return Vector2<T>(x + r.x, y + r.y);
  }
  template<class T>
  Vector2<T> Vector2<T>::operator-(const Vector2<T> &r) const {
    return Vector2<T>(x - r.x, y - r.y);
  }

  template<class T>
  Vector2<T> Vector2<T>::operator*(T factor) const {
    return Vector2<T>(x * factor, y * factor);
  }
  template<class T>
  Vector2<T> Vector2<T>::operator/(T factor) const {
    return Vector2<T>(x / factor, y / factor);
  }

  template<class T>
  Vector2<T> &Vector2<T>::operator+=(const Vector2<T> &r) {
    x += r.x;
    y += r.y;
    return *this;
  }
  template<class T>
  Vector2<T> &Vector2<T>::operator-=(const Vector2<T> &r) {
    x -= r.x;
    y -= r.y;
    return *this;
  }

  template<class T>
  Vector2<T> &Vector2<T>::operator*=(T factor) {
    x *= factor;
    y *= factor;
    return *this;
  }
  template<class T>
  Vector2<T> &Vector2<T>::operator/=(T factor) {
    x /= factor;
    y /= factor;
    return *this;
  }

  template<class T>
  Vector2<T> &Vector2<T>::operator=(const Vector2<T> &other) {
    x = other.x;
    y = other.y;
    return *this;
  }

  template<class T> 
  template<class U>
  Vector2<U> Vector2<T>::Convert() const {
    return Vector2<U>((U) x, (U) y);
  }

  template<class T>
  T Vector2<T>::Length() const {
    return std::sqrt(x * x + y * y);
  }
  template<class T>
  Vector2<T> Vector2<T>::Direction() const {
    return *this / Length();
  }
}

#endif // PX_DISASTER_SYSTEM_VECTOR2_HPP