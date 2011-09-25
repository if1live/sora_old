#pragma once

#include "matsu/vector_template.h"

namespace matsu
{
///@brief 여러종류의 행렬을 적젏하게 처리하기 위해서 만든 클래스이다.
///인자로 row, col을 명시할때는 행-열로 받지만
///내부에서 저장은 col-row순으로 되면(opengl친화적)
///배열을 인자로 받을때도 그렇게 처리된다
template<typename T, int row, int col>
class MatrixTemplate {
public:
  enum {
    Row = row,
    Col = col,
  };
public:
  MatrixTemplate();
  template<unsigned int N>
  MatrixTemplate(const T(&data)[N]);
  template<int row2, int col2>
  MatrixTemplate(const MatrixTemplate<T,row2,col2> &o);
  template<typename T2>
  MatrixTemplate(const MatrixTemplate<T2,row,col> &o);
  ~MatrixTemplate();

  T Get(int r, int c) const;
  void Set(int r, int c, T value);
  template<unsigned int N>
  void SetMatrix(const T(&data)[N]);
  void SetMatrix(const T *data);

  T* Pointer();
  const T* Pointer() const;

  //operator
  MatrixTemplate<T,row,col> operator+(const MatrixTemplate<T,row,col> &o) const;
  MatrixTemplate<T,row,col> &operator+=(const MatrixTemplate<T,row,col> &o);
  MatrixTemplate<T,row,col> operator-(const MatrixTemplate<T,row,col> &o) const;
  MatrixTemplate<T,row,col> &operator-=(const MatrixTemplate<T,row,col> &o);
  MatrixTemplate<T,row,col> operator-() const;
  ///@brief *=는 자신의 값 자체가 바뀌므로 행렬이 정행렬일때만 가능하다
  template<int size>
  MatrixTemplate<T,size,size> operator*=(const MatrixTemplate<T,size,size> &o);
  template<int col2>
  MatrixTemplate<T,row,col2> operator*(const MatrixTemplate<T,col,col2> &o) const;
  MatrixTemplate<T,row,col> operator*(T s) const;
  MatrixTemplate<T,row,col> &operator*=(T s);

  VectorTemplate<T,row> operator*(const VectorTemplate<T,col> &o) const;

  bool operator==(const MatrixTemplate<T,row,col> &o) const;
  bool operator!=(const MatrixTemplate<T,row,col> &o) const;

  VectorTemplate<T,col> GetRow(int r) const;
  VectorTemplate<T,row> GetCol(int c) const;
  void SetRow(int r, const VectorTemplate<T,col> &o);
  void SetCol(int c, const VectorTemplate<T,row> &o);

  MatrixTemplate<T,col,row> Transpose() const;

  int GetIndex(int r, int c) const;
  T data[row*col];
};
}

#include "matsu/matrix_template.hpp"