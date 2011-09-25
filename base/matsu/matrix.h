// Ŭnicode please
#pragma once
#include "matsu/matrix_template.h"

namespace matsu
{
typedef MatrixTemplate<int,2,2> imat2;
typedef MatrixTemplate<float,2,2> mat2;
typedef MatrixTemplate<int,3,3> imat3;
typedef MatrixTemplate<float,3,3> mat3;
typedef MatrixTemplate<int,4,4> imat4;
typedef MatrixTemplate<float,4,4> mat4;
}

namespace matsu
{
class Matrix {
public:
  template<typename T>
  static MatrixTemplate<T,4,4> Identity();

  template<typename T>
  static MatrixTemplate<T,4,4> Translate(T x, T y, T z);

  template<typename T>
  static MatrixTemplate<T,4,4> Scale(T s);

  template<typename T>
  static MatrixTemplate<T,4,4> Scale(T x, T y, T z);

  template<typename T>
  static MatrixTemplate<T,4,4> RotateX(T degrees);
    
  template<typename T>
  static MatrixTemplate<T,4,4> RotateY(T degrees);
    
  template<typename T>
  static MatrixTemplate<T,4,4> RotateZ(T degrees);

  template<typename T>
  static MatrixTemplate<T,4,4> xAxisShear(T y, T z);

  template<typename T>
  static MatrixTemplate<T,4,4> yAxisShear(T x, T z);

  template<typename T>
  static MatrixTemplate<T,4,4> zAxisShear(T z, T y);

  ///#pragma mark camera
  template<typename T>
  static MatrixTemplate<T,4,4> Ortho(T left, 
    T right, T bottom, T top, T nearVal, T farVal
    );

  template<typename T>
  static MatrixTemplate<T,4,4> Frustum(T left, 
    T right, T bottom, T top, T nearVal, T farVal
    );

  template<typename T>
  static MatrixTemplate<T,4,4> LookAt(
    const matsu::VectorTemplate<T,3> &eye, 
    const matsu::VectorTemplate<T,3> &target, 
    const matsu::VectorTemplate<T,3> &up
    );
};
}

#include "matsu/matrix.hpp"