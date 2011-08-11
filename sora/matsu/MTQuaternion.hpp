// Ŭnicode please
#pragma once

#include "matsu/MTMath.h"

namespace matsu
{
	template <typename T>
	inline QuaternionTpl<T>::QuaternionTpl() : x(0), y(0), z(0), w(1)
	{
	}

	template <typename T>
	inline QuaternionTpl<T>::QuaternionTpl(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
	{
	}

	// Ken Shoemake's famous method.
	template <typename T>
	inline QuaternionTpl<T> QuaternionTpl<T>::slerp(T t, const QuaternionTpl<T>& v1) const
	{
		const T epsilon = 0.0005f;
		T dotvalue = dot(v1);

		if (dotvalue > 1 - epsilon) {
			QuaternionTpl<T> result = v1 + (*this - v1).scaled(t);
			result.normalized();
			return result;
		}

		if (dotvalue < 0)
			dotvalue = 0;

		if (dotvalue > 1)
			dotvalue = 1;

		T theta0 = std::acos(dotvalue);
		T theta = theta0 * t;

		QuaternionTpl<T> v2 = (v1 - scaled(dotvalue));
		v2.Normalize();

		QuaternionTpl<T> q = scaled(std::cos(theta)) + v2.scaled(std::sin(theta));
		q.Normalize();
		return q;
	}

	template <typename T>
	inline QuaternionTpl<T> QuaternionTpl<T>::rotated(const QuaternionTpl<T>& b) const
	{
		QuaternionTpl<T> q;
		q.w = w * b.w - x * b.x - y * b.y - z * b.z;
		q.x = w * b.x + x * b.w + y * b.z - z * b.y;
		q.y = w * b.y + y * b.w + z * b.x - x * b.z;
		q.z = w * b.z + z * b.w + x * b.y - y * b.x;
		q.Normalize();
		return q;
	}

	template <typename T>
	inline QuaternionTpl<T> QuaternionTpl<T>::scaled(T s) const
	{
		return QuaternionTpl<T>(x * s, y * s, z * s, w * s);
	}

	template <typename T>
	inline T QuaternionTpl<T>::dot(const QuaternionTpl<T>& q) const
	{
		return x * q.x + y * q.y + z * q.z + w * q.w;
	}

	template <typename T>
	inline MatrixTpl<T,3,3> QuaternionTpl<T>::toMatrix() const
	{
		const T s = 2;
		T xs, ys, zs;
		T wx, wy, wz;
		T xx, xy, xz;
		T yy, yz, zz;
		xs = x * s;  ys = y * s;  zs = z * s;
		wx = w * xs; wy = w * ys; wz = w * zs;
		xx = x * xs; xy = x * ys; xz = x * zs;
		yy = y * ys; yz = y * zs; zz = z * zs;
		MatrixTpl<T,3,3> m;
		m.x.x = 1 - (yy + zz); m.y.x = xy - wz;  m.z.x = xz + wy;
		m.x.y = xy + wz; m.y.y = 1 - (xx + zz); m.z.y = yz - wx;
		m.x.z = xz - wy; m.y.z = yz + wx;  m.z.z = 1 - (xx + yy);
		return m;
	}

	template <typename T>
	inline VectorTpl<T,4> QuaternionTpl<T>::toVector() const
	{
		return VectorTpl<T,4>(x, y, z, w);
	}

	template <typename T>
	QuaternionTpl<T> QuaternionTpl<T>::operator-(const QuaternionTpl<T>& q) const
	{
		return QuaternionTpl<T>(x - q.x, y - q.y, z - q.z, w - q.w);
	}

	template <typename T>
	QuaternionTpl<T> QuaternionTpl<T>::operator+(const QuaternionTpl<T>& q) const
	{
		return QuaternionTpl<T>(x + q.x, y + q.y, z + q.z, w + q.w);
	}

	template <typename T>
	bool QuaternionTpl<T>::operator==(const QuaternionTpl<T>& q) const
	{
		return x == q.x && y == q.y && z == q.z && w == q.w;
	}

	template <typename T>
	bool QuaternionTpl<T>::operator!=(const QuaternionTpl<T>& q) const
	{
		return !(*this == q);
	}
	
	template<typename T>
	QuaternionTpl<T> QuaternionTpl<T>::operator*(const QuaternionTpl<T> &rq) const
	{
		// the constructor takes its arguments as (x, y, z, w)
		return Quaternion(w * rq.x + x * rq.w + y * rq.z - z * rq.y,
						  w * rq.y + y * rq.w + z * rq.x - x * rq.z,
						  w * rq.z + z * rq.w + x * rq.y - y * rq.x,
						  w * rq.w - x * rq.x - y * rq.y - z * rq.z);
	}
	
	template<typename T>
	VectorTpl<T,3> QuaternionTpl<T>::operator*(const VectorTpl<T,3> &v) const
	{
		//http://gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation#Quaternion_from_axis-angle
		VectorTpl<T,3> vn(v);
		vn.normalized();
		
		QuaternionTpl<T> vecQuat;
		QuaternionTpl<T> resQuat;
		vecQuat.x = vn.getX();
		vecQuat.y = vn.getY();
		vecQuat.z = vn.getZ();
		vecQuat.w = 0.0f;
		
		resQuat = vecQuat * conjugation();
		resQuat = *this * resQuat;
		
		return VectorTpl<T,3>(resQuat.x, resQuat.y, resQuat.z);
	}

	// Compute the quaternion that rotates from a to b, avoiding numerical instability.
	// Taken from "The Shortest Arc Quaternion" by Stan Melax in "Game Programming Gems".
	template <typename T>
	inline QuaternionTpl<T> QuaternionTpl<T>::createFromVectors(const VectorTpl<T,3>& v0, const VectorTpl<T,3>& v1)
	{
		if (v0 == -v1)
		{
			#ifndef M_PI
			#define M_PI 3.141592
			return QuaternionTpl<T>::createFromAxisAngle(vec3(1, 0, 0), M_PI);
			#endif
		}

		VectorTpl<T,3> c = v0.cross(v1);
		T d = v0.dot(v1);
		T s = std::sqrt((1 + d) * 2);

		QuaternionTpl<T> q;
		q.x = c.x / s;
		q.y = c.y / s;
		q.z = c.z / s;
		q.w = s / 2.0f;
		return q;
	}

	template <typename T>
	inline QuaternionTpl<T>  QuaternionTpl<T>::createFromAxisAngle(const VectorTpl<T,3>& axis, float radians)
	{
		//http://gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation#Quaternion_from_axis-angle
		QuaternionTpl<T> q;
		q.w = std::cos(radians / 2);
		q.x = q.y = q.z = std::sin(radians / 2);
		q.x *= axis.getX();
		q.y *= axis.getY();
		q.z *= axis.getZ();
		return q;
	}

	template <typename T>
	inline void QuaternionTpl<T>::normalized()
	{
		*this = scaled(1 / std::sqrt(dot(*this)));
	}

	template <typename T>
	inline void QuaternionTpl<T>::rotate(const QuaternionTpl<T>& q2)
	{
		QuaternionTpl<T> q;
		QuaternionTpl<T>& q1 = *this;

		q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
		q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
		q.y = q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z;
		q.z = q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x;

		q.normalized();
		*this = q;
	}

	template<typename T>
	QuaternionTpl<T> QuaternionTpl<T>::conjugation() const
	{
		QuaternionTpl<T> q = *this;
		q.x = -q.x;
		q.y = -q.y;
		q.z = -q.z;
		return q;
	}

	template<typename T>
	QuaternionTpl<T> QuaternionTpl<T>::inverse() const
	{
		QuaternionTpl<T> q = conjugation();
		q.normalized();
		return q;
	}
}
