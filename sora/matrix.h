// Copyright (C) 2011 by if1live
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#ifndef SORA_MATRIX_H_
#define SORA_MATRIX_H_

#if SR_USE_PCH == 0
#include <cstring>
#include <cmath>
#endif
#include "sora/assert_inc.h"
#include "sora/vector.h"
#include "sora/math_helper.h"

namespace sora {;
//0 2
//1 3
typedef float Mat2f[4];
typedef Mat2f Mat2;

//0 3 6
//1 4 7
//2 5 8
typedef float Mat3f[9];
typedef Mat3f Mat3;

//0 4 8  12
//1 5 9  13
//2 6 10 14
//3 7 11 15
typedef float Mat4f[16];
typedef Mat4f Mat4;

template<int D2>
struct Dim {
  enum { value = 1 };
};
template<>
struct Dim<4> {
  enum { value = 2 };
};
template<>
struct Dim<9> {
  enum { value = 3 };
};
template<>
struct Dim<16> {
  enum { value = 4 };
};

//test
template <typename T, int D1, int D2>
struct flatten_array {
	enum { array_size = D1 * D2 };
	typedef T type[array_size];
};

template<typename T, int D1, int D2>
typename flatten_array<T, D1, D2>::type& flatten(T (&arg)[D1][D2]) {
	return (typename flatten_array<T, D1, D2>::type&)arg;
}

template<int size>
void print(int (&arg)[size]) {
	for (int i = 0; i < size; i++) {
		printf("%d", arg[i]);
	}
}

template<typename T, int D2>
struct twodim_array {
  enum { array_size = D2, dim = Dim<D2>::value };
  typedef T type[dim][dim];
};

template<typename T, int D2>
typename twodim_array<T, D2>::type& twodim(T (&arg)[D2]) {
  return (typename twodim_array<T, D2>::type&)arg;
}

template<int size>
void print(int (&arg)[size][size]) {
	for (int i = 0; i < size; i++) {
    for(int j = 0 ; j < size ; j++) {
		  printf("%d", arg[i][j]);
    }
	}
}

////////

template<int D, int R, int C>
int GetIndex() {
  return (D*C + R);
}
template<int D>
int GetIndex(int r, int c) {
  return (D*c + r);
}

template<typename T, int D2>
void MatSetIdentity(T (&m)[D2]) {
  memset(m, 0, sizeof(T) * D2);
  const int D = Dim<D2>::value;
  for (int i = 0 ; i < D ; i++) {
    m[i*D + i] = 1;
  }
}

template<typename T, int D2>
bool MatIsIdentity(T (&m)[D2]) {
  T identity[D2];
  MatSetIdentity(identity);
  if(memcmp(m, identity, sizeof(identity)) == 0) {
    return true;
  } else {
    return false;
  }
}

template<typename T, int D2>
void MatAdd(const T (&a)[D2], const T (&b)[D2], T (&c)[D2]) {
  for(int i = 0 ; i < D2 ; i++) {
    c[i] = a[i] + b[i];
  }
}

template<typename T, int D2>
void MatSub(const T (&a)[D2], const T (&b)[D2], T (&c)[D2]) {
  for(int i = 0 ; i < D2 ; i++) {
    c[i] = a[i] - b[i];
  }
}

template<typename T, int D2>
void MatCopy(const T (&src)[D2], T (&dst)[D2]) {
  memcpy(dst, src, sizeof(T) * D2);
}


template<typename T, int D2>
bool MatEq(const T (&a)[D2], const T (&b)[D2]) {
  if(memcmp(a, b, sizeof(T) * D2) == 0) {
    return true;
  } else {
    return false;
  }
}

template<typename T, int D2>
bool MatNotEq(const T (&a)[D2], const T (&b)[D2]) {
  return !MatEq(a, b);
}


template<typename T, int D2>
void MatMult(const T (&a)[D2], T b, T (&c)[D2]) {
  for(int i = 0 ; i < D2 ; i++) {
    c[i] = a[i] * b;
  }
}

template<typename T, int D2, int X>
void MatGetRow(int r, const T (&m)[D2], T (&retval)[X]) {
  const int D = Dim<D2>::value;
  for (int i = 0 ; i < D ; i++) {
    retval[i] = m[GetIndex<D>(r, i)];
  }
}
template<typename T, int D2, int X>
void MatGetCol(int c, const T (&m)[D2], T (&retval)[X]) {
  const int D = Dim<D2>::value;
  for (int i = 0 ; i < D ; i++) {
    retval[i] = m[GetIndex<D>(i, c)];
  }
}
template<typename T, int D2, int X>
void MatSetRow(int r, T (&data)[X], T (&m)[D2]) {
  const int D = Dim<D2>::value;
  for (int i = 0 ; i < D ; i++) {
    m[GetIndex<D>(r, i)] = data[i];
  }
}

template<typename T, int D2, int X>
void MatSetCol(int c, T (&data)[X], T (&m)[D2])  {
  const int D = Dim<D2>::value;
  for (int i = 0 ; i < D ; i++) {
    m[GetIndex<D>(i, c)] = data[i];
  }
}

template<typename T, int D2>
void MatTranspose(const T (&in)[D2], T (&out)[D2]) {
  T tmp[D2];
  memcpy(tmp, in, sizeof(tmp));
  const int D = Dim<D2>::value;
  for(int i = 0 ; i < D ; i ++) {
    for(int j = 0 ; j < D ; j++) {
      out[GetIndex<D>(j, i)] = tmp[GetIndex<D>(i, j)];
    }
  }
}


template<typename T, int D2, int D>
void MatMult(const T (&m)[D2], const T (&b)[D], T (&retval)[D]) {
  retval[0] = m[GetIndex<D,0,0>()]*b[0] + m[GetIndex<D, 1, 0>()]*b[1] + m[GetIndex<D, 2, 0>()]*b[2] + m[GetIndex<D, 3, 0>()]*b[3];
  retval[1] = m[GetIndex<D,0,1>()]*b[0] + m[GetIndex<D, 1, 1>()]*b[1] + m[GetIndex<D, 2, 1>()]*b[2] + m[GetIndex<D, 3, 1>()]*b[3];
  retval[2] = m[GetIndex<D,0,2>()]*b[0] + m[GetIndex<D, 1, 2>()]*b[1] + m[GetIndex<D, 2, 2>()]*b[2] + m[GetIndex<D, 3, 2>()]*b[3];
  retval[3] = m[GetIndex<D,0,3>()]*b[0] + m[GetIndex<D, 1, 3>()]*b[1] + m[GetIndex<D, 2, 3>()]*b[2] + m[GetIndex<D, 3, 3>()]*b[3];
}

template<typename T, int D2>
void MatMult(const T (&a)[D2], const T (&b)[D2], T (&result)[D2]) {
  T tmp[D2];
  const int D = Dim<D2>::value;
  for (int i = 0 ; i < D ; i++) {
    for (int j = 0 ; j < D ; j++) {

      T row_data[D];
      T col_data[D];
      MatGetRow(i, a, row_data);
      MatGetCol(j, b, col_data);

      T value = 0;
      for (int k = 0 ; k < D ; k++) {
        value += row_data[k] * col_data[k];
      }

      tmp[GetIndex<D>(i, j)] = value;
    }
  }
  memcpy(result, tmp, sizeof(tmp));
}

template<typename T>
void Translate(T x, T y, T z, T (&m)[16]) {
  T data[] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    x, y, z, 1
  };
  memcpy(m, data, sizeof(data));
}

template<typename T>
void Scale(T x, T y, T z, T (&m)[16]) {
  T data[] = {
    x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, 1
  };
  memcpy(m, data, sizeof(data));
}

template<typename T>
void Scale(T s, T (&m)[16]) {
  Scale(s, s, s, m);
}

template<typename T>
void RotateX(T degrees, T (&m)[16]) {
  T radians = degrees * 3.14159f / 180.0f;
  T s = std::sin(radians);
  T c = std::cos(radians);
  T data[] = {
    1, 0, 0, 0,
    0, c, s, 0,
    0, -s, c, 0,
    0, 0, 0, 1
  };
  memcpy(m, data, sizeof(data));
}

template<typename T>
void RotateY(T degrees, T (&m)[16]) {
  T radians = degrees * 3.14159f / 180.0f;
  T s = std::sin(radians);
  T c = std::cos(radians);
  T data[] = {
    c, 0, -s, 0,
    0, 1, 0, 0,
    s, 0, c, 0,
    0, 0, 0, 1,
  };
  memcpy(m, data, sizeof(data));
}

template<typename T>
void RotateZ(T degrees, T (&m)[16]) {
  T radians = degrees * 3.14159f / 180.0f;
  T s = std::sin(radians);
  T c = std::cos(radians);
  T data[] = {
    c, s, 0, 0,
    -s, c, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  };
  memcpy(m, data, sizeof(data));
}

template<typename T>
void Ortho(T left, T right, T bottom, T top, T near_val, T far_val, T (&m)[16]) {
  T a = static_cast<T>(2) / (right - left);
  T b = static_cast<T>(2) / (top - bottom);
  T c = static_cast<T>(2) / (far_val - near_val);
  T tx = (right+left) / (right-left);
  T ty = (top+bottom) / (top-bottom);
  T tz = (far_val+near_val) / (far_val-near_val);

  T data[] = {
    a, 0, 0, 0,
    0, b, 0, 0, 
    0, 0, c, 0,
    -tx, -ty, -tz, 1
  };
  memcpy(m, data, sizeof(data));
}

template<typename T>
void Frustum(T left, T right, T bottom, T top, T near_val, T far_val, T (&m)[16]) {
  T a = static_cast<T>(2) * near_val / (right - left);
  T b = static_cast<T>(2) * near_val / (top - bottom);
  T c = (right + left) / (right - left);
  T d = (top + bottom) / (top - bottom);
  T e = - (far_val + near_val) / (far_val - near_val);
  T f = -static_cast<T>(2) * far_val * near_val / (far_val - near_val);
  T data[] = {
    a, 0, 0, 0,
    0, b, 0, 0,
    c, d, e, -1,
    0, 0, f, 1
  };
  memcpy(m, data, sizeof(data));
}

template<typename T>
void LookAt(const T (&eye)[3], const T (&target)[3], const T (&up)[3], T (&result)[16]) {
  float m[16];
  float x[3], y[3], z[3];
  float mag;

  /* Make rotation matrix */

  /* Z vector */
  z[0] = eye[0] - target[0];
  z[1] = eye[1] - target[1];
  z[2] = eye[2] - target[2];
  mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
  if (mag) {                      /* mpichler, 19950515 */
    z[0] /= mag;
    z[1] /= mag;
    z[2] /= mag;
  }

  /* Y vector */
  y[0] = up[0];
  y[1] = up[1];
  y[2] = up[2];

  /* X vector = Y cross Z */
  x[0] = y[1] * z[2] - y[2] * z[1];
  x[1] = -y[0] * z[2] + y[2] * z[0];
  x[2] = y[0] * z[1] - y[1] * z[0];

  /* Recompute Y = Z cross X */
  y[0] = z[1] * x[2] - z[2] * x[1];
  y[1] = -z[0] * x[2] + z[2] * x[0];
  y[2] = z[0] * x[1] - z[1] * x[0];

  /* mpichler, 19950515 */
  /* cross product gives area of parallelogram, which is < 1.0 for
  * non-perpendicular unit-length vectors; so normalize x, y here
  */

  mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
  if (mag) {
    x[0] /= mag;
    x[1] /= mag;
    x[2] /= mag;
  }

  mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
  if (mag) {
    y[0] /= mag;
    y[1] /= mag;
    y[2] /= mag;
  }

#define M(row,col)  m[col*4+row]
  M(0, 0) = x[0];
  M(0, 1) = x[1];
  M(0, 2) = x[2];
  M(0, 3) = 0.0;
  M(1, 0) = y[0];
  M(1, 1) = y[1];
  M(1, 2) = y[2];
  M(1, 3) = 0.0;
  M(2, 0) = z[0];
  M(2, 1) = z[1];
  M(2, 2) = z[2];
  M(2, 3) = 0.0;
  M(3, 0) = 0.0;
  M(3, 1) = 0.0;
  M(3, 2) = 0.0;
  M(3, 3) = 1.0;
#undef M

  Mat4f m2;
  Translate(-eye[0], -eye[1], -eye[2], m2);

  MatMult(m, m2, result);
}

template<typename T>
void Perspective(T fovy, T aspect, T zNear, T zFar, T (&m)[16]) {
  ///http://www.opengl.org/wiki/GluPerspective_code
  float ymax, xmax;
  //float temp, temp2, temp3, temp4;
  ymax = zNear * tanf((float)(fovy * kPi / 360.0));
  //ymin = -ymax;
  //xmin = -ymax * aspectRatio;
  xmax = ymax * aspect;
  Frustum(-xmax, xmax, -ymax, ymax, zNear, zFar, m);
}

}

#endif  // SORA_MATRIX_H_