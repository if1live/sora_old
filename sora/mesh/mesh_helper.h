﻿/*  Copyright (C) 2011-2012 by if1live */
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
#ifndef SORA_MESH_HELPER_H_
#define SORA_MESH_HELPER_H_

namespace sora {;
template<typename VertexType>
class MeshHelper {
public:
  //triangles로 구성된 vertex list+index list를 입력으로 받아서
  //normal을 적절히 생성
  //normal의 범위는 -1~1 float인 기본형으로
  static void BuildNormal(std::vector<VertexType> &vert_list, const std::vector<unsigned short> &index_list);
  
  //triangles로 구성된 vetex list+index list를 입력받아서
  //tangent적절히 생성
  static void BuildTangent(std::vector<VertexType> &vert_list, const std::vector<unsigned short> &index_list);

  static void ResetNormal(std::vector<VertexType> &vert_list);
  static void AddNormal(std::vector<VertexType> &vert_list, const std::vector<unsigned short> &index_list);
  static void NormalizeNormal(std::vector<VertexType> &vert_list);
};

//impl

template<typename VertexType>
void MeshHelper<VertexType>::ResetNormal(std::vector<VertexType> &vert_list) {
  auto vert_it = vert_list.begin();
  auto vert_endit = vert_list.end();
  for( ; vert_it != vert_endit ; ++vert_it) {
    VertexType &vert = *vert_it;
    vert.normal = vec3(0.0f);
  }
}

template<typename VertexType>
void MeshHelper<VertexType>::NormalizeNormal(std::vector<VertexType> &vert_list) {
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
void MeshHelper<VertexType>::AddNormal(std::vector<VertexType> &vert_list, const std::vector<unsigned short> &index_list) {
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
void MeshHelper<VertexType>::BuildNormal(std::vector<VertexType> &vert_list, const std::vector<unsigned short> &index_list) {
  //reset prev normal
  ResetNormal(vert_list);
  AddNormal(vert_list, index_list);
  NormalizeNormal(vert_list);
}

template<typename VertexType>
void MeshHelper<VertexType>::BuildTangent(std::vector<VertexType> &vert_list, const std::vector<unsigned short> &index_list) {
}

} //namespace sora

#endif  // SORA_MESH_HELPER_H_