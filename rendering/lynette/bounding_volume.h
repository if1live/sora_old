/*  Copyright (C) 2011 by if1live */
//
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
#ifndef RENDERING_LYNETTE_BOUNDING_VOLUME_H_
#define RENDERING_LYNETTE_BOUNDING_VOLUME_H_

#include "lynette/lynette_enum.h"

namespace lynette {;
template<typename T>
class BoundingVolume {
public:
  BoundingVolume();
  ~BoundingVolume() {}

  void AddVertex(T x, T y, T z);
  void AddModel(const sora::model::Model &model);

public:
  T left;
  T right;
  T top;
  T bottom;
  T front;
  T back;
  T getWidth() const { return right - left; }
  T getHeight() const { return top - bottom; }
  T getDepth() const { return front - back; }
};
}

namespace lynette {;
template<typename T>
BoundingVolume::BoundingVolume()
  :	left(INT_MAX),
  right(-INT_MAX),
  bottom(INT_MAX),
  top(-INT_MAX),
  front(-INT_MAX),
  back(INT_MAX) {
}

template<typename T>
void BoundingVolume::AddVertex(T x, T y, T z) {
    if (x < left) {
      left = x;
    }
    if (x > right) {
      right = x;
    }
    if (y < bottom) {
      bottom = y;
    }
    if (y > top) {
      top = y;
    }
    if (z > front) {
      front = z;
    }
    if (z < back) {
      back = z;
    }
}
template<typename T>
void BoundingVolume::AddModel(const sora::model::Model &model) {
    const float *pos_ptr = model.getPosPtr();
  int stride = model.getStride();
  int vertex_cnt = model.getVertexCount();

  for (int i = 0 ; i < vertex_cnt ; i++)   {
    ///3d니가 float3개로 좌표를 표현할 것이다 
    int index = (stride / sizeof(float)) * i;
    const float *vertex_ptr = pos_ptr + index;
    float x = vertex_ptr[0];
    float y = vertex_ptr[1];
    float z = vertex_ptr[2];
    AddVertex(x, y, z);
  }
}
}

#endif  // RENDERING_LYNETTE_BOUNDING_VOLUME_H_