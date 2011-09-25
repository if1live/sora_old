// Ŭnicode please
#pragma once

#include "matsu/vector.h"
#include "matsu/matrix.h"

namespace matsu
{
template <typename T>
class QuaternionTemplate {
public:
	T x;
	T y;
	T z;
	T w;

	QuaternionTemplate();
	QuaternionTemplate(T x, T y, T z, T w);

	QuaternionTemplate<T> sLerp(T mu, const QuaternionTemplate<T>& q) const;
	QuaternionTemplate<T> rotated(const QuaternionTemplate<T>& b) const;
	QuaternionTemplate<T> scaled(T scale) const;
	T dot(const QuaternionTemplate<T>& q) const;
	MatrixTemplate<T,3,3> toMatrix() const;
	VectorTemplate<T,4> toVector() const;
	QuaternionTemplate<T> operator-(const QuaternionTemplate<T>& q) const;
	QuaternionTemplate<T> operator+(const QuaternionTemplate<T>& q) const;
	bool operator==(const QuaternionTemplate<T>& q) const;
	bool operator!=(const QuaternionTemplate<T>& q) const;
		
	QuaternionTemplate<T> operator*(const QuaternionTemplate<T> &ro) const;
	VectorTemplate<T,3> operator*(const VectorTemplate<T,3> &v) const;

	void normalized();
	void rotate(const QuaternionTemplate<T>& q);
		
	QuaternionTemplate<T> conjugation() const;
	QuaternionTemplate<T> inverse() const;

	static QuaternionTemplate<T> CreateFromVectors(const VectorTemplate<T,3>& v0, const VectorTemplate<T,3>& v1);
	static QuaternionTemplate<T> CreateFromAxisAngle(const VectorTemplate<T,3>& axis, float radians);
};

//typedef
typedef QuaternionTemplate<float> Quaternion;	//일반적으로 쓸거같은거
typedef QuaternionTemplate<float> QuaternionF;
typedef QuaternionTemplate<float> QuaternionD;
typedef QuaternionTemplate<float> quat;
}

#include "matsu/quaternion.hpp"
