#pragma once

namespace matsu
{
	//순수하게 템플릿만을 사용해서 다양한 타입을 받는 여러차원의 벡터 클래스 구현
	template<typename T, int dim>
	class VectorTpl {
	public:
		enum { dimension = dim };
	public:
		///@brief 영벡터를 만드는 생성자
		VectorTpl();
		///@brief 배열을 이용해서 초기화하는 생성자
		template<unsigned int N>
		VectorTpl(const T (&data)[N]);
		///@brief 차원은 같고 내부 데이터 타입이 다른 벡터를 사용하는 생성자
		template<typename T2>
		VectorTpl(const VectorTpl<T2,dim> &o);
		///@brief 타입은 같지만 차원이 다른 벡터를 사용하는 생성자. 만약 인자로 받는게
		///높은 차원이면 버린다. 낮은 차원이면 추가되는 차원의 항은 0으로
		template<int dim2>
		VectorTpl(const VectorTpl<T,dim2> &o);
		///@brief 차원이 될때까지 채우고 남는것은 etc로 채우기
		template<int dim2>
		VectorTpl(const VectorTpl<T,dim2> &o, T etc);
		///@brief 값을 직접 받아서 생성하기. 넘치거나 모자르는 항은 0으로
		VectorTpl(T x, T y);
		VectorTpl(T x, T y, T z);
		VectorTpl(T x, T y, T z, T w);
		~VectorTpl();
		
		template<int index> 
		void set(T value);
		void set(int index, T value);
		template<unsigned int N>
		void set(const T (&data)[N]);
		void setVector(T x, T y);
		void setVector(T x, T y, T z);
		void setVector(T x, T y, T z, T w);
		
		template<int index>
		T get() const;
		T get(int index) const;
		
		//operator overloading
		VectorTpl<T,dim> operator+(const VectorTpl<T,dim> &o) const;
		VectorTpl<T,dim>& operator+=(const VectorTpl<T,dim> &o);
		VectorTpl<T,dim> operator-(const VectorTpl<T,dim> &o) const;
		VectorTpl<T,dim>& operator-=(const VectorTpl<T,dim> &o);
		VectorTpl<T,dim> operator-() const;
		VectorTpl<T,dim> operator*(T t) const;
		VectorTpl<T,dim>& operator*=(T t);
		VectorTpl<T,dim> operator/(T t) const;
		VectorTpl<T,dim>& operator/=(T t);
		bool operator==(const VectorTpl<T,dim> &o) const;
		bool operator!=(const VectorTpl<T,dim> &o) const;
		
		T dot(const VectorTpl<T,dim> &o) const;
		///@brief 수학적으로 외적은 3차원 벡터끼리 수행해서 결과가 3차원 벡터로 나온다
		///해당 벡터 클래스는 무제한 차원을 노리고 만든것이니까 this에 해당하는것의 차원은
		///제한할수없지만 인자와 반환값은 무조건 3차원으로 고정하자
		VectorTpl<T,3> cross(const VectorTpl<T,3> &o) const;
		
		T size() const;
		T sizeSquare() const;
		
		VectorTpl<T,dim> normalize() const;
		void normalized();
		
		const T* pointer() const;
		T* pointer();
		
		///@brief enum에서 정의된 dimension만 써도 문제없을거같은데 문제가 생길떄가 있어서 함수도 만들었다
		int getDim() const;
		
		bool isZero(T errorRange=0.001) const;
		
		///@brief 요청한 값에 해당하는 차원이 존재하는가
		bool isValidIndex(int index) const;
		
		//freq used
#define SHORT_GETTER(NAME, index)	\
T NAME() const { MT_ASSERT(index < dim); { return get<index>(); } }
		SHORT_GETTER(x, 0);
		SHORT_GETTER(y, 1);
		SHORT_GETTER(z, 2);
		SHORT_GETTER(w, 3);
		SHORT_GETTER(getX, 0);
		SHORT_GETTER(getY, 1);
		SHORT_GETTER(getZ, 2);
		SHORT_GETTER(getW, 3);
#undef SHORT_GETTER
		
#define SHORT_SETTER(NAME, index)	\
void NAME(T value) { MT_ASSERT(index < dim); { set<index>(value); } }
		SHORT_SETTER(setX, 0);
		SHORT_SETTER(setY, 1);
		SHORT_SETTER(setZ, 2);
		SHORT_SETTER(setW, 3);
#undef SHORT_SETTER
		
	private:
		T data_[dim];
	};
	
	typedef VectorTpl<int,2> ivec2;
	typedef VectorTpl<float,2> vec2;
	
	typedef VectorTpl<int,3> ivec3;
	typedef VectorTpl<float,3> vec3;
	
	typedef VectorTpl<int,4> ivec4;
	typedef VectorTpl<float,4> vec4;
}

#include "matsu/MTVectorTpl.hpp"