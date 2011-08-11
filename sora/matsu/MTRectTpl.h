// Ŭnicode please
#pragma once

namespace matsu
{
	template<typename T> class RectTpl;
	typedef RectTpl<float> Rect;
	typedef RectTpl<int> Recti;
	typedef RectTpl<float> Rectf;

	template<typename T>
	class RectTpl {
	public:
		RectTpl(T x, T y, T w, T h);
		~RectTpl();

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
	RectTpl<T>::RectTpl(T x, T y, T w, T h)
		: x_(x), y_(y), width_(w), height_(h)
	{
	}
	template<typename T>
	RectTpl<T>::~RectTpl()
	{
	}
	template<typename T>
	T RectTpl<T>::getX() const
	{
		return x_;
	}
	template<typename T>
	T RectTpl<T>::getY() const
	{
		return y_;
	}
	template<typename T>
	T RectTpl<T>::getWidth() const
	{
		return width_;
	}
	template<typename T>
	T RectTpl<T>::getHeight() const
	{
		return height_;
	}
}