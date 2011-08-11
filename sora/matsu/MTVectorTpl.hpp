#pragma once

#include "matsu/MTMacro.h"

//boost static assert를 써서 컴파일 타입에 인덱스를 보장함(2차원 벡터에서는 z얻기를 못한다든가하는거)
//를 사용할 경우, getter, setter의 자동생성시 뻑난다. 그냥 무한차원중에서 앞부분만 쓰는 느낌으로 하자
//#include <boost/static_assert.hpp

//template impl
namespace matsu
{
	template<typename T, int dim>
	VectorTpl<T,dim>::VectorTpl()
	{
		memset(data_, 0, sizeof(T) * dim);
	}
	template<typename T, int dim>
	template<unsigned int N>
	VectorTpl<T,dim>::VectorTpl(const T (&data)[N])
	{
		set(data);
	}
	template<typename T, int dim>
	template<typename T2>
	VectorTpl<T,dim>::VectorTpl(const VectorTpl<T2,dim> &o)
	{
		for(int i = 0 ; i < dim ; i++)
		{
			data_[i] = static_cast<T> (o.get(i));
		}
	}
	template<typename T, int dim>
	template<int dim2>
	VectorTpl<T,dim>::VectorTpl(const VectorTpl<T,dim2> &o)
	{
		memset(data_, 0, sizeof(T) * dim);
		for(int i = 0 ; i < dim && i < dim2 ; i++)
		{
			data_[i] = static_cast<T> (o.get(i));
		}
	}
	template<typename T, int dim>
	template<int dim2>
	VectorTpl<T,dim>::VectorTpl(const VectorTpl<T,dim2> &o, T etc)
	{
		memset(data_, 0, sizeof(T) * dim);
		int i = 0;
		for(i = 0 ; i < dim && i < dim2 ; i++)
		{
			data_[i] = static_cast<T> (o.get(i));
		}
		for( ; i < dim ; i++)
		{
			data_[i] = etc;
		}
	}
	template<typename T, int dim>
	VectorTpl<T,dim>::VectorTpl(T x, T y)
	{
		setVector(x, y);
	}
	template<typename T, int dim>
	VectorTpl<T,dim>::VectorTpl(T x, T y, T z)
	{
		setVector(x, y, z);
	}
	template<typename T, int dim>
	VectorTpl<T,dim>::VectorTpl(T x, T y, T z, T w)
	{
		setVector(x, y, z, w);
	}
	template<typename T, int dim>
	void VectorTpl<T,dim>::setVector(T x, T y)
	{
		memset(data_, 0, sizeof(T) * dim);
		if(isValidIndex(0))
		{
			data_[0] = x;
		}
		if(isValidIndex(1))
		{
			data_[1] = y;
		}
	}
	template<typename T, int dim>
	void VectorTpl<T,dim>::setVector(T x, T y, T z)
	{
		memset(data_, 0, sizeof(T) * dim);
		if(isValidIndex(0))
		{
			data_[0] = x;
		}
		if(isValidIndex(1))
		{
			data_[1] = y;
		}
		if(isValidIndex(2))
		{
			data_[2] = z;
		}
	}
	template<typename T, int dim>
	void VectorTpl<T,dim>::setVector(T x, T y, T z, T w)
	{
		memset(data_, 0, sizeof(T) * dim);
		if(isValidIndex(0))
		{
			data_[0] = x;
		}
		if(isValidIndex(1))
		{
			data_[1] = y;
		}
		if(isValidIndex(2))
		{
			data_[2] = z;
		}
		if(isValidIndex(3))
		{
			data_[3] = w;
		}
	}
	
	template<typename T, int dim>
	VectorTpl<T,dim>::~VectorTpl()
	{
	}
	
	template<typename T, int dim>
	template<int index> 
	void VectorTpl<T,dim>::set(T value)
	{
		data_[index] = value;
	}
	
	template<typename T, int dim>
	void VectorTpl<T,dim>::set(int index, T value)
	{
		data_[index] = value;
	}
	template<typename T, int dim>
	template<unsigned int N>
	void VectorTpl<T,dim>::set(const T (&data)[N])
	{
		int i = 0;
		for(i = 0 ; i < N && i < dim ; i++)
		{
			data_[i] = data[i];
		}
		//길이가 모자르고 남은부분은 0으로
		for( ; i < dim ; i++)
		{
			data_[i] = 0;
		}
	}
	
	template<typename T, int dim>
	template<int index>
	T VectorTpl<T,dim>::get() const
	{
		return data_[index];
	}
	
	template<typename T, int dim>
	T VectorTpl<T,dim>::get(int index) const
	{
		return data_[index];
	}
	
	template<typename T, int dim>
	VectorTpl<T,dim> VectorTpl<T,dim>::operator+(const VectorTpl<T,dim> &o) const
	{
		VectorTpl<T,dim> tmp(*this);
		tmp += o;
		return tmp;
	}
	
	/*
	 //+=의 내부 구현을 tmp로 하기 위해서 이런걸 쓰면 어떨가?
	 template<typename T, int index, int dim>
	 struct NumAdd {
	 enum { value = NumAdd<T,index-1,dim>::value };
	 };
	 template<typename T, int dim>
	 struct NumAdd<T, 0, dim> {
	 enum { value = 1 };
	 };
	 */
	
	
	template<typename T, int dim>
	VectorTpl<T,dim>& VectorTpl<T,dim>::operator+=(const VectorTpl<T,dim> &o)
	{
		for(int i = 0 ; i < dim ; i++) 
		{
			data_[i] += o.data_[i];
		}
		return *this;
	}
	template<typename T, int dim>
	VectorTpl<T,dim> VectorTpl<T,dim>::operator-(const VectorTpl<T,dim> &o) const
	{
		VectorTpl<T,dim> tmp(*this);
		tmp -= o;
		return tmp;
	}
	template<typename T, int dim>
	VectorTpl<T,dim>& VectorTpl<T,dim>::operator-=(const VectorTpl<T,dim> &o)
	{
		for(int i = 0 ; i < dim ; i++) 
		{
			data_[i] -= o.data_[i];
		}
		return *this;
	}
	template<typename T, int dim>
	VectorTpl<T,dim> VectorTpl<T,dim>::operator-() const
	{
		VectorTpl<T,dim> result;	//그냥 생성하면0
		result -= *this;
		return result;
	}
	template<typename T, int dim>
	VectorTpl<T,dim> VectorTpl<T,dim>::operator*(T t) const
	{
		VectorTpl<T,dim> tmp(*this);
		tmp *= t;
		return tmp;
	}
	template<typename T, int dim>
	VectorTpl<T,dim>& VectorTpl<T,dim>::operator*=(T t)
	{
		for(int i = 0 ; i < dim ; i++) 
		{
			data_[i] *= t;
		}
		return *this;
	}
	template<typename T, int dim>
	VectorTpl<T,dim> VectorTpl<T,dim>::operator/(T t) const
	{
		VectorTpl<T,dim> tmp(*this);
		tmp /= t;
		return tmp;
	}
	template<typename T, int dim>
	VectorTpl<T,dim>& VectorTpl<T,dim>::operator/=(T t)
	{
		for(int i = 0 ; i < dim ; i++) 
		{
			data_[i] /= t;
		}
		return *this;
	}
	
	template<typename T, int dim>
	T VectorTpl<T,dim>::dot(const VectorTpl<T,dim> &o) const
	{
		T result = 0;
		for(int i = 0 ; i < dim ; i++)
		{
			result += get(i) * o.get(i);
		}
		return result;
	}
	
	template<typename T, int dim>
	VectorTpl<T,3> VectorTpl<T,dim>::cross(const VectorTpl<T,3> &o) const
	{
		MT_ASSERT(dim == 3);
		T x = get<1>() * o.get<2>() - get<2>() * o.get<1>();
		T y = get<2>() * o.get<0>() - get<0>() * o.get<2>();
		T z = get<0>() * o.get<1>() - get<1>() * o.get<0>();
		return VectorTpl<T,3>(x, y, z);
	}
	
	template<typename T, int dim>
	T VectorTpl<T,dim>::size() const
	{
		return ::sqrtf(static_cast<T>(sizeSquare()));
	}
	template<typename T, int dim>
	T VectorTpl<T,dim>::sizeSquare() const
	{
		T result = 0;
		for(int i = 0 ; i < dim ; i++)
		{
			result += get(i) * get(i);
		}
		return result;
	}
	
	template<typename T, int dim>
	VectorTpl<T,dim> VectorTpl<T,dim>::normalize() const
	{
		VectorTpl<T,dim> tmp = *this;
		tmp.normalized();
		return tmp;
	}
	
	template<typename T, int dim>
	void VectorTpl<T,dim>::normalized()
	{
		float s = 1.0f / size();
		VectorTpl<T,dim> &tmp = *this;
		tmp *= s;
	}
	
	template<typename T, int dim>
	bool VectorTpl<T,dim>::isZero(T errorRange) const
	{
		T error = 1;
		for(int i = 0 ; i < dim ; i++)
		{
			error *= errorRange;
		}
		return (error > sizeSquare());
	}
	
	template<typename T, int dim>
	bool VectorTpl<T,dim>::operator==(const VectorTpl<T,dim> &o) const
	{
		for(int i = 0 ; i < dim ; i++)
		{
			if(get(i) != o.get(i))
			{
				return false;
			}
		}
		return true;
	}
	
	template<typename T, int dim>
	bool VectorTpl<T,dim>::operator!=(const VectorTpl<T,dim> &o) const
	{
		return !(*this == o);
	}
	
	template<typename T, int dim>
	int VectorTpl<T,dim>::getDim() const
	{
		return dim;
	}
	
	template<typename T, int dim>
	T* VectorTpl<T,dim>::pointer()
	{
		return data_;
	}
	template<typename T, int dim>
	const T* VectorTpl<T,dim>::pointer() const
	{
		return data_;
	}
	
	template<typename T, int dim>
	bool VectorTpl<T,dim>::isValidIndex(int index) const
	{
		return (dim > index);
	}
}