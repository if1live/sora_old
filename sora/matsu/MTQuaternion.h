// Ŭnicode please
#pragma once

#include "matsu/MTVector.h"
#include "matsu/MTMatrix.h"

namespace matsu
{
	template <typename T>
	class QuaternionTpl {
	public:
		T x;
		T y;
		T z;
		T w;

		QuaternionTpl();
		QuaternionTpl(T x, T y, T z, T w);

		QuaternionTpl<T> slerp(T mu, const QuaternionTpl<T>& q) const;
		QuaternionTpl<T> rotated(const QuaternionTpl<T>& b) const;
		QuaternionTpl<T> scaled(T scale) const;
		T dot(const QuaternionTpl<T>& q) const;
		MatrixTpl<T,3,3> toMatrix() const;
		VectorTpl<T,4> toVector() const;
		QuaternionTpl<T> operator-(const QuaternionTpl<T>& q) const;
		QuaternionTpl<T> operator+(const QuaternionTpl<T>& q) const;
		bool operator==(const QuaternionTpl<T>& q) const;
		bool operator!=(const QuaternionTpl<T>& q) const;
		
		QuaternionTpl<T> operator*(const QuaternionTpl<T> &ro) const;
		VectorTpl<T,3> operator*(const VectorTpl<T,3> &v) const;

		void normalized();
		void rotate(const QuaternionTpl<T>& q);
		
		QuaternionTpl<T> conjugation() const;
		QuaternionTpl<T> inverse() const;

		static QuaternionTpl<T> createFromVectors(const VectorTpl<T,3>& v0, const VectorTpl<T,3>& v1);
		static QuaternionTpl<T> createFromAxisAngle(const VectorTpl<T,3>& axis, float radians);
	};

	//typedef
	typedef QuaternionTpl<float> Quaternion;	//일반적으로 쓸거같은거
	typedef QuaternionTpl<float> QuaternionF;
	typedef QuaternionTpl<float> QuaternionD;
	typedef QuaternionTpl<float> quat;
}

#include "matsu/MTQuaternion.hpp"
