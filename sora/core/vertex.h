/*  Copyright (C) 2011-2012 by if1live */
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
#ifndef SORA_VERTEX_H_
#define SORA_VERTEX_H_

#include "core/vector.h"
#include "core/array_inc.h"
#include "glm/core/type_vec.hpp"

namespace sora {;

template<
  typename PosT,
  int PosD,
  typename TexcoordT,
  int TexcoordD,
  typename NormalT,
  int NormalD,
  typename ColorT,
  int ColorD,
  typename TangentT,
  int TangentD
> struct VertexT;

typedef VertexT<
  float, 3, 
  float, 2,
  float, 3,
  unsigned char, 4,
  float, 0
> Vertex;

typedef VertexT<
  float, 3, 
  float, 2,
  float, 3,
  unsigned char, 4,
  float, 3
> TangentVertex;

typedef VertexT<
  float, 2,
  float, 2,
  float, 0,
  float, 0,
  float, 0
> Vertex2D;

typedef std::vector<Vertex> VertexList;
typedef std::vector<TangentVertex> TangentVertexList;
typedef std::vector<Vertex2D> Vertex2DList;

typedef unsigned short IndexType;
typedef std::vector<unsigned short> IndexList;


template<typename VertexType>
struct VertexListSelector {
};
template<>
struct VertexListSelector<Vertex> {
  typedef VertexList Result;
};
template<>
struct VertexListSelector<Vertex2D> {
  typedef Vertex2DList Result;
};
template<>
struct VertexListSelector<TangentVertex> {
  typedef TangentVertexList Result;
};


template<typename T>
struct VertexMaxValue { enum { value = 1 }; };
template<>
struct VertexMaxValue<unsigned char> { enum { value = 255 }; };

//glm::vecX로 초기화 가능한 배열. 이것을 기반으로 vertex를 구성하면 glm도 바로 쓸수잇다
template<typename T, int D>
struct ArrayT {
  typedef T value_type;
  ArrayT() {
    for(int i = 0 ; i < D ; ++i) {
      data[i] = 0;
    }
  }

  template< template<typename T> class VecType >
  ArrayT(const VecType<T> &v) {
    ArrayT &self = *this;
    self = v;
  }

  template< template<typename T> class VecType >
  ArrayT &operator=(const VecType<T> &v) {
    //static_assert(D == 4, "dim not same");
    for(int i = 0 ; i < D ; ++i) {
      data[i] = v[i];
    }
    return *this;
  }

  T &operator[](int idx) { return data[idx]; }

  template< template<typename T> class VecType >
  VecType<T> Get() {
    VecType<T> result;
    for(int i = 0 ; i < D ; ++i) {
      result[i] = data[i];
    }
    return result;
  }

  std::array<T, D> data;
};

template<
  typename PosT,
  int PosD,
  typename TexcoordT,
  int TexcoordD,
  typename NormalT,
  int NormalD,
  typename ColorT,
  int ColorD,
  typename TangentT,
  int TangentD
> struct VertexT {

  typedef PosT PosType;
  typedef TexcoordT TexcoordType;
  typedef NormalT NormalType;
  typedef ColorT ColorType;
  typedef TangentT TangentType;

  enum {
    PosDim = PosD,
    ColorDim = ColorD,
    TexcoordDim = TexcoordD,
    NormalDim = NormalD,
    TangentDim = TangentD,
  };

  VertexT() {
    static_assert(PosD >= 2, "not valid pos dim");
    static_assert(NormalD == 0 || NormalD == 3, "not valid tangent dim");
    static_assert(TangentD == 0 || TangentD == 3, "not valid tangent dim");
    Init();
  }
  void Init() {
    for(int i = 0 ; i < PosD ; ++i) {
      pos[i] = 0;
    }
    for(int i = 0 ; i < TexcoordD ; ++i) {
      texcoord[i] = 0;
    }
    for(int i = 0 ; i < NormalD ; ++i) {
      if(i == 0) {
        normal[i] = 1;
      } else {
        normal[i] = 0;
      }
    }
    for(int i = 0 ; i < ColorD ; ++i) {
      color[i] = VertexMaxValue<ColorT>::value;
    }
    for(int i = 0 ; i < TangentD ; ++i) {
      tangent[i] = 0;
    }
  }
  ArrayT<PosT, PosD> pos;
  ArrayT<TexcoordT, TexcoordD> texcoord;
  ArrayT<NormalT, NormalD> normal;
  ArrayT<ColorT, ColorD> color;
  ArrayT<TangentT, TangentD> tangent;

  void set_pos(const glm::detail::tvec3<PosT> &v) { pos = v; }
  void set_normal(const glm::detail::tvec3<NormalT> &v) { normal = v; }
  void set_tangent(const glm::detail::tvec3<TangentT> &v) { tangent = v; }
  void set_texcoord(const glm::detail::tvec2<TexcoordT> &v) { texcoord = v; }
};

//버텍스 생성을 조금더 쉽게 하기 위한 함수
Vertex CreateVertex(const glm::vec3 &pos, const glm::vec2 &texcoord);
Vertex2D CreateVertex2D(float x, float y, float s, float t);

/*
template<typename VertexType>
struct VertexHelper {
  //triangles로 구성된 vertex list+index list를 입력으로 받아서
  //normal을 적절히 생성
  //normal의 범위는 -1~1 float인 기본형으로
  static void BuildNormal(std::vector<VertexType> &vert_list, const IndexList &index_list);

  //triangles로 구성된 vetex list+index list를 입력받아서
  //tangent적절히 생성
  static void BuildTangent(std::vector<VertexType> &vert_list, const IndexList &index_list);

  static void ResetNormal(std::vector<VertexType> &vert_list);
  static void AddNormal(std::vector<VertexType> &vert_list, const IndexList &index_list);
  static void NormalizeNormal(std::vector<VertexType> &vert_list);
};


template<typename VertexType>
void VertexHelper<VertexType>::ResetNormal(std::vector<VertexType> &vert_list) {
  auto vert_it = vert_list.begin();
  auto vert_endit = vert_list.end();
  for( ; vert_it != vert_endit ; ++vert_it) {
    VertexType &vert = *vert_it;
    vert.normal = vec3(0.0f);
  }
}

template<typename VertexType>
void VertexHelper<VertexType>::NormalizeNormal(std::vector<VertexType> &vert_list) {
  //normalize normal
  auto vert_it = vert_list.begin();
  auto vert_endit = vert_list.end();
  for( ; vert_it != vert_endit ; ++vert_it) {
    VertexType &vert = *vert_it;
    glm::vec3 normal = glm::normalize(vert.normal);
    vert.normal = normal;
  }
}

template<typename VertexType>
void VertexHelper<VertexType>::AddNormal(std::vector<VertexType> &vert_list, const IndexList &index_list) {
  //면단위로 노멀 계산해서 더하기
  //ccw기준으로 인덱스가 넘어올것이라고 전제 깔기
  for(size_t i = 0 ; i < index_list.size() / 3 ; ++i) {
    int idx1 = index_list[i*3 + 0];
    int idx2 = index_list[i*3 + 1];
    int idx3 = index_list[i*3 + 2];

    VertexType &v1 = vert_list[idx1];
    VertexType &v2 = vert_list[idx2];
    VertexType &v3 = vert_list[idx3];

    glm::vec3 p1 = v2.pos - v1.pos;
    glm::vec3 p2 = v3.pos - v2.pos;
    glm::vec3 cross_value = glm::cross(p1, p2);
    glm::vec3 normal = glm::normalize(cross_value);

    v1.normal += normal;
    v2.normal += normal;
    v3.normal += normal;
  }
}

template<typename VertexType>
void VertexHelper<VertexType>::BuildNormal(std::vector<VertexType> &vert_list, const IndexList &index_list) {
  //reset prev normal
  ResetNormal(vert_list);
  AddNormal(vert_list, index_list);
  NormalizeNormal(vert_list);
}

template<typename VertexType>
void VertexHelper<VertexType>::BuildTangent(std::vector<VertexType> &vert_list, const IndexList &index_list) {
}
*/
}

#endif  // SORA_VERTEX_H_