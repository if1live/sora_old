#pragma once

//template impl
namespace matsu
{
template<typename T, int row, int col>
template<typename T2>
MatrixTemplate<T,row,col>::MatrixTemplate(const MatrixTemplate<T2,row,col> &o)
{
  for(int i = 0 ; i < row*col ; i++) {
    data[i] = static_cast<T> (o.data[i]);
  }
}

template<typename T, int row, int col>
MatrixTemplate<T,row,col>::MatrixTemplate()
{
  memset(data, 0, sizeof(T) * row * col);
  int minValue = row > col ? col : row;
  for(int i = 0 ; i < minValue ; i++) {
    Set(i, i, 1);
  }
}
template<typename T, int row, int col>
template<unsigned int N>
MatrixTemplate<T,row,col>::MatrixTemplate(const T(&data)[N])
{
  SetMatrix(data);
}
template<typename T, int row, int col>
template<int row2, int col2>
MatrixTemplate<T,row,col>::MatrixTemplate(const MatrixTemplate<T,row2,col2> &o)
{
  memset(data, 0, sizeof(T) * row * col);
  for(int i = 0 ; i < row2 && i < row ; i++){
    for(int j = 0 ; j < col && j < col2 ; j++) {
      Set(i, j, o.Get(i, j));
    }
  }
}

template<typename T, int row, int col>
MatrixTemplate<T,row,col>::~MatrixTemplate()
{
}

template<typename T, int row, int col>
template<unsigned int N>
void MatrixTemplate<T,row,col>::SetMatrix(const T(&m)[N])
{
  SR_ASSERT(N == row*col);
  memcpy(data, m, sizeof(T) * row * col);
}
template<typename T,int row, int col>
void MatrixTemplate<T,row,col>::SetMatrix(const T *m)
{
  //data의 크기가 보장되어있지 않은게 좀 걸리는데...
  memcpy(data, m, sizeof(T) * row * col);
}

template<typename T, int row, int col>
T MatrixTemplate<T,row,col>::Get(int r, int c) const
{
  return data[GetIndex(r, c)];
}

template<typename T, int row, int col>
void MatrixTemplate<T,row,col>::Set(int r, int c, T value)
{
  data[GetIndex(r, c)] = value;
}

template<typename T, int row, int col>
T* MatrixTemplate<T,row,col>::Pointer()
{
  return data;
}

template<typename T, int row, int col>
const T* MatrixTemplate<T,row,col>::Pointer() const
{
  return data;
}

template<typename T, int row, int col>
int MatrixTemplate<T,row,col>::GetIndex(int r, int c) const
{
  int index = c*row + r;
  SR_ASSERT(index >= 0 && index < row*col);
  return index;
}
template<typename T, int row, int col>
MatrixTemplate<T,row,col> MatrixTemplate<T,row,col>::operator+(const MatrixTemplate<T,row,col> &o) const
{
  MatrixTemplate<T,row,col> tmp = *this;
  tmp += o;
  return tmp;
}
template<typename T, int row, int col>
MatrixTemplate<T,row,col> &MatrixTemplate<T,row,col>::operator+=(const MatrixTemplate<T,row,col> &o)
{
  for(int i = 0 ; i < row*col ; i++) {
    data[i] += o.data[i];
  }
}
template<typename T, int row, int col>
MatrixTemplate<T,row,col> MatrixTemplate<T,row,col>::operator-(const MatrixTemplate<T,row,col> &o) const
{
  MatrixTemplate<T,row,col> tmp = *this;
  tmp -= o;
  return tmp;
}
template<typename T, int row, int col>
MatrixTemplate<T,row,col> &MatrixTemplate<T,row,col>::operator-=(const MatrixTemplate<T,row,col> &o)
{
  for(int i = 0 ; i < row*col ; i++) {
    data[i] -= o.data[i];
  }
}

template<typename T, int row, int col>
MatrixTemplate<T,row,col> MatrixTemplate<T,row,col>::operator-() const
{
  MatrixTemplate<T,row,col> tmp;
  for(int i = 0 ; i < row*col ; i++) {
    tmp.data[i] -= data[i];
  }
  return tmp;
}

template<typename T, int row, int col>
bool MatrixTemplate<T,row,col>::operator==(const MatrixTemplate<T,row,col> &o) const
{
  for(int i = 0 ; i < row*col ; i++) {
    if(data[i] != o.data[i]) {
      return false;
    }
  }
  return true;
}

template<typename T, int row, int col>
bool MatrixTemplate<T,row,col>::operator!=(const MatrixTemplate<T,row,col> &o) const
{
  return !(*this == o);
}

template<typename T, int row, int col>
MatrixTemplate<T,col,row> MatrixTemplate<T,row,col>::Transpose() const
{
  MatrixTemplate<T,col,row> result;
  for(int i = 0 ; i < row ; i++) {
    for(int j = 0 ; j < col ; j++) {
      result.Set(j, i, Get(i, j));
    }
  }
  return result;
}

template<typename T, int row, int col>
VectorTemplate<T,col> MatrixTemplate<T,row,col>::GetRow(int r) const
{
  VectorTemplate<T,col> result;
  for (int i = 0 ; i < col ; i++) {
    result.data[i] = Get(r, i);
  }
  return result;
}
template<typename T, int row, int col>
VectorTemplate<T,row> MatrixTemplate<T,row,col>::GetCol(int c) const
{
  VectorTemplate<T,row> result;
  for(int i = 0 ; i < row ; i++) {
    result.data[i] = Get(i, c);
  }
  return result;
}

template<typename T, int row, int col>
void MatrixTemplate<T,row,col>::SetRow(int r, const VectorTemplate<T,col> &o)
{
  for(int i = 0 ; i < col ; i++) {
    Set(r, i, o.Get(i));
  }
}

template<typename T, int row, int col>
void MatrixTemplate<T,row,col>::SetCol(int c, const VectorTemplate<T,row> &o)
{
  for(int i = 0 ; i < row ; i++) {
    Set(i, c, o.Get(i));
  }
}

template<typename T, int row, int col>
template<int size>
MatrixTemplate<T,size,size> MatrixTemplate<T,row,col>::operator*=(const MatrixTemplate<T,size,size> &o)
{
  MatrixTemplate<T,size,size> result;
  SR_ASSERT(row == col && "support only X*X matrix");
  for(int i = 0 ; i < size ; i++) {
    for(int j = 0 ; j < size ; j++) {
      VectorTemplate<T,size> rowVec = GetRow(i);
      VectorTemplate<T,size> colVec = o.GetCol(j);
      T value = rowVec.Dot(colVec);
      result.Set(i, j, value);
    }
  }
  memcpy(data, result.data, sizeof(T) * row * col);
}
template<typename T, int row, int col>
template<int col2>
MatrixTemplate<T,row,col2> MatrixTemplate<T,row,col>::operator*(const MatrixTemplate<T,col,col2> &o) const
{
  MatrixTemplate<T,row,col2> result;
  for(int i = 0 ; i < row ; i++) {
    for(int j = 0 ; j < col2 ; j++) {
      VectorTemplate<T,col> rowVec = GetRow(i);
      VectorTemplate<T,col> colVec = o.GetCol(j);
      T value = rowVec.Dot(colVec);
      result.Set(i, j, value);
    }
  }
  return result;
}

template<typename T, int row, int col>
MatrixTemplate<T,row,col> MatrixTemplate<T,row,col>::operator*(T s) const
{
  MatrixTemplate<T,row,col> tmp = *this;
  tmp *= s;
  return tmp;
}

template<typename T, int row, int col>
MatrixTemplate<T,row,col> &MatrixTemplate<T,row,col>::operator*=(T s)
{
  for(int i = 0 ; i < row*col ; i++)
  {
    data[i] *= s;
  }
  return *this;
}

template<typename T, int row, int col>
VectorTemplate<T,row> MatrixTemplate<T,row,col>::operator*(const VectorTemplate<T,col> &o) const
{
  T data[row];
  for(int i = 0 ; i < row ; i++) {
    VectorTemplate<T,col> rowVec = GetRow(i);
    data[i] = rowVec.Dot(o);
  }
  return VectorTemplate<T,row>(data);
}
}