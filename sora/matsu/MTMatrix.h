// Ŭnicode please
#pragma once
#include "matsu/MTMatrixTpl.h"

namespace matsu
{
	namespace mat
	{
		///#pragma mark transform
		template<typename T>
		MatrixTpl<T,4,4> identity();
		
		template<typename T>
		MatrixTpl<T,4,4> translate(T x, T y, T z);
		
		template<typename T>
		MatrixTpl<T,4,4> scale(T s);
		template<typename T>
		MatrixTpl<T,4,4> scale(T x, T y, T z);
		
		template<typename T>
		MatrixTpl<T,4,4> xRotate(T degrees);
		template<typename T>
		MatrixTpl<T,4,4> yRotate(T degrees);
		template<typename T>
		MatrixTpl<T,4,4> zRotate(T degrees);
		
		template<typename T>
		MatrixTpl<T,4,4> xAxisShear(T y, T z);
		template<typename T>
		MatrixTpl<T,4,4> yAxisShear(T x, T z);
		template<typename T>
		MatrixTpl<T,4,4> zAxisShear(T z, T y);
		
		///#pragma mark camera
		template<typename T>
		MatrixTpl<T,4,4> ortho(T left, T right, T bottom, T top, T nearVal, T farVal);
		
		template<typename T>
		MatrixTpl<T,4,4> frustum(T left, T right, T bottom, T top, T nearVal, T farVal);
		
		template<typename T>
		MatrixTpl<T,4,4> lookAt(const matsu::VectorTpl<T,3> &eye, const matsu::VectorTpl<T,3> &target, const matsu::VectorTpl<T,3> &up);
	}
}

#include "matsu/MTMatrix.hpp"