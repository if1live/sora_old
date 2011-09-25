#pragma once

#include "sora/macro.h"

namespace matsu
{
//순수하게 템플릿만을 사용해서 다양한 타입을 받는 여러차원의 벡터 클래스 구현
template<typename T, int D>
class VectorTemplate {
public:
  enum { Dimension = D };
public:
  ///@brief 영벡터를 만드는 생성자
  VectorTemplate();

  ///@brief 배열을 이용해서 초기화하는 생성자
  template<unsigned int N>
  VectorTemplate(const T (&input)[N]);

  ///@brief 차원은 같고 내부 데이터 타입이 다른 벡터를 사용하는 생성자
  template<typename T2>
  VectorTemplate(const VectorTemplate<T2,D> &o);

  ///@brief 타입은 같지만 차원이 다른 벡터를 사용하는 생성자. 만약 인자로 받는게
  ///높은 차원이면 버린다. 낮은 차원이면 추가되는 차원의 항은 0으로
  template<int D2>
  VectorTemplate(const VectorTemplate<T,D2> &o);

  ///@brief 차원이 될때까지 채우고 남는것은 etc로 채우기
  template<int D2>
  VectorTemplate(const VectorTemplate<T,D2> &o, T etc);

  ///@brief 값을 직접 받아서 생성하기. 넘치거나 모자르는 항은 0으로
  VectorTemplate(T x, T y);
  VectorTemplate(T x, T y, T z);
  VectorTemplate(T x, T y, T z, T w);
  ~VectorTemplate();

  template<unsigned int N>
  void SetVector(const T (&input)[N]);

  void SetVector(T x, T y);
  void SetVector(T x, T y, T z);
  void SetVector(T x, T y, T z, T w);

  //operator overloading
  VectorTemplate<T,D> operator+(const VectorTemplate<T,D> &o) const;
  VectorTemplate<T,D>& operator+=(const VectorTemplate<T,D> &o);
  VectorTemplate<T,D> operator-(const VectorTemplate<T,D> &o) const;
  VectorTemplate<T,D>& operator-=(const VectorTemplate<T,D> &o);
  VectorTemplate<T,D> operator-() const;
  VectorTemplate<T,D> operator*(T t) const;
  VectorTemplate<T,D>& operator*=(T t);
  VectorTemplate<T,D> operator/(T t) const;
  VectorTemplate<T,D>& operator/=(T t);
  bool operator==(const VectorTemplate<T,D> &o) const;
  bool operator!=(const VectorTemplate<T,D> &o) const;

  T Dot(const VectorTemplate<T,D> &o) const;

  ///@brief 수학적으로 외적은 3차원 벡터끼리 수행해서 결과가 3차원 벡터로 나온다
  ///해당 벡터 클래스는 무제한 차원을 노리고 만든것이니까 this에 해당하는것의 차원은
  ///제한할수없지만 인자와 반환값은 무조건 3차원으로 고정하자
  VectorTemplate<T,3> Cross(const VectorTemplate<T,3> &o) const;

  T Size() const;
  T SizeSquare() const;

  VectorTemplate<T,D> Normalize() const;
  void Normalized();

  const T* Pointer() const;
  T* Pointer();

  ///@brief enum에서 정의된 dimension만 써도 문제없을거같은데 문제가 생길떄가 있어서 함수도 만들었다
  int GetDimension() const;

  bool IsZero(T errorRange=0.001) const;

  ///@brief 요청한 값에 해당하는 차원이 존재하는가
  bool IsValidIndex(int index) const;

  //freq used
#define SHORT_GETTER(NAME, index)	\
T NAME() const { SR_ASSERT(index < D); { return data[index]; } }
  SHORT_GETTER(GetX, 0);
  SHORT_GETTER(GetY, 1);
  SHORT_GETTER(GetZ, 2);
  SHORT_GETTER(GetW, 3);
#undef SHORT_GETTER

#define SHORT_SETTER(NAME, index)	\
void NAME(T value) { SR_ASSERT(index < D); { data[index] = value; } }
  SHORT_SETTER(SetX, 0);
  SHORT_SETTER(SetY, 1);
  SHORT_SETTER(SetZ, 2);
  SHORT_SETTER(SetW, 3);
#undef SHORT_SETTER

public:
  T data[D];
};
}

#include "matsu/vector_template.hpp"