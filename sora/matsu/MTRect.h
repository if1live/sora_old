// Ŭnicode please
#pragma once

namespace matsu
{
	template<typename T> class Rect;
	typedef Rect<int> Recti;
	typedef Rect<float> Rectf;

	template<typename T>
	class Rect {
	public:
		Rect(T x, T y, T w, T h);
		~Rect();

		T getX() const;
		T getY() const;
		T getWidth() const;
		T getHeight() const;
	private:
		T x_;
		T y_;
		T width_;
		T height_;
	};
}

namespace matsu
{
	template<typename T>
	Rect<T>::Rect(T x, T y, T w, T h)
		: x_(x), y_(y), width_(w), height_(h)
	{
	}
	template<typename T>
	Rect<T>::~Rect()
	{
	}
	template<typename T>
	T Rect<T>::getX() const
	{
		return x_;
	}
	template<typename T>
	T Rect<T>::getY() const
	{
		return y_;
	}
	template<typename T>
	T Rect<T>::getWidth() const
	{
		return width_;
	}
	template<typename T>
	T Rect<T>::getHeight() const
	{
		return height_;
	}
}