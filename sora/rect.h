/*  Copyright (C) 2011 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#ifndef SORA_RECT_H_
#define SORA_RECT_H_

#include "vector.h"

namespace sora {;
template<typename T>  class Rect;
typedef Rect<float> Rectf;
typedef Rect<int> Recti;


//점 2개로 사각형을 표현하는 클래스이다. Rectangle와는 다르게 주로 데이터 저장을 목적으로 사용한다
template<typename T>
class Rect {
public:
  Rect() {}
  Rect(T x, T y, T width, T height) : origin_(x, y), size_(width, height) {}
	template<typename T2>
	Rect(const Rect<T2> &o);
	Rect(const sora::Vector2<T> &origin, const sora::Vector2<T> &size)
    : origin_(origin), size_(size)  {}
  ~Rect() {}

	const sora::Vector2<T> &origin() const { return origin_; }
	const sora::Vector2<T> &size() const { return size_; }
	T width() const { return size_.x; }
	T height() const { return size_.y; }
	T x() const { return origin_.x; }
	T y() const { return origin_.y; }

	template<typename T1>
	operator Rect<T1>() const;

	bool operator==(const Rect<T> &o) const;
	bool operator!=(const Rect<T> &o) const;

  bool IsEmpty() const;
private:
	sora::Vector2<T> origin_;
	sora::Vector2<T> size_;
};
}

//template impl
namespace sora {;

template<typename T>
template<typename T2>
Rect<T>::Rect(const Rect<T2> &o) {
	T x = static_cast<T>(o.x);
	T y = static_cast<T>(o.y);
	T w = static_cast<T>(o.width);
	T h = static_cast<T>(o.height);
	origin_.x = x;
	origin_.y = y;
	size_.x = w;
	size_.y = h;
}

template<typename T>
bool Rect<T>::operator==(const Rect<T> &o) const {
	return (origin_ == o.origin_ && size_ == o.size_);
}
template<typename T>
bool Rect<T>::operator!=(const Rect<T> &o) const {
	return !(*this == o);
}

template<typename T>
template<typename T1>
Rect<T>::operator Rect<T1>() const {
	T1 x = static_cast<T1>(origin_.x);
	T1 y = static_cast<T1>(origin_.y);
	T1 w = static_cast<T1>(size_.x);
	T1 h = static_cast<T1>(size_.y);
	Rect<T1> result(x, y, w, h);
	return result;
}
template<typename T>
bool Rect<T>::IsEmpty() const {
  return (x() == 0 && y() == 0 && width() == 0 && height() == 0);
}
}

#endif  // SORA_RECT_H_