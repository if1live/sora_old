// Ŭnicode please
#pragma once

namespace matsu
{
template<typename T> class Rectangle;
typedef Rectangle<int> Recti;
typedef Rectangle<float> Rectf;

template<typename T>
class Rectangle {
public:
  typedef T Type;
public:
	Rectangle(T x, T y, T w, T h);
	~Rectangle();
	
  T x;
	T y;
	T width;
	T height;
};
}

namespace matsu
{
template<typename T>
Rectangle<T>::Rectangle(T x, T y, T w, T h)
	: x(x), y(y), width(w), height(h)
{
}
template<typename T>
Rectangle<T>::~Rectangle()
{
}
}