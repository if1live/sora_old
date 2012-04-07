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
#include "sora_stdafx.h"
#include "primitive_model_builder.h"
#include "core/math_helper.h"

using namespace std;
using namespace glm;

namespace sora {;

PrimitiveModelBuilder::PrimitiveModelBuilder(unsigned int flag)
: flag_(flag) {
  SetCube(1, 1, 1);
}

void PrimitiveModelBuilder::SetCube(float width, float height, float depth) {
  SR_ASSERT(width > 0 && height > 0 && depth > 0);
  //선그리는건 위치정보만 뱉으면 충분하다
  SR_ASSERT(flag_ == 0);
  cube.width = width;
  cube.height = height;
  cube.depth = depth;
}
void PrimitiveModelBuilder::SetSphere(float radius, int slices, int stacks) {
  SR_ASSERT(radius > 0);
  SR_ASSERT(slices > 0);
  SR_ASSERT(stacks > 0);
  //선그리는건 위치정보만 뱉으면 충분하다
  SR_ASSERT(flag_ == 0);
  sphere.radius = radius;
  sphere.slices = slices;
  sphere.stacks = stacks;
}

int PrimitiveModelBuilder::CalcOffset(unsigned int flag, int *pos, int *color, int *normal, int *texcoord, int *tangent) {
  pos = 0;
  int curr_offset = 3;  //pos는 항상 들어가니까
  if(flag & kFlagColor) {
    *color = curr_offset;
    curr_offset += 4;  //색깔은 속성 4개
  } else {
    *color = -1;
  }

  if(flag & kFlagNormal) {
    *normal = curr_offset;
    curr_offset += 3;
  } else {
    *normal = -1;
  }

  if(flag & kFlagTexcoord) {
    *texcoord  = curr_offset;
    curr_offset += 2;
  } else {
    *texcoord = -1;
  }

  if(flag & kFlagTangent) {
    *tangent = curr_offset;
    curr_offset += 3;
  } else {
    *tangent = -1;
  }
  return curr_offset;
}

void PrimitiveModelBuilder::DataToVertexList(const std::vector<float> &data, unsigned int flag, std::vector<Vertex> &vert_list) {
  DataToCommonVertexList(data, flag, vert_list);
}
void PrimitiveModelBuilder::DataToVertexList(const std::vector<float> &data, unsigned int flag, std::vector<TangentVertex> &vert_list) {
  int pos_offset = 0;
  int color_offset = -1;
  int normal_offset = -1;
  int texcoord_offset = -1;
  int tangent_offset = -1;
  int rowsize = CalcOffset(flag, &pos_offset, &color_offset, &normal_offset, &texcoord_offset, &tangent_offset);

  //공통부분 읽기
  DataToCommonVertexList(data, flag, vert_list);
  for(size_t i = 0 ; i < data.size() / rowsize ; ++i) {
    TangentVertex &vert = vert_list[i];
    if(tangent_offset != -1) {
      int tangent_idx = i * rowsize + tangent_offset;
      vert.tangent.x = data[tangent_idx + 0];
      vert.tangent.y = data[tangent_idx + 1];
      vert.tangent.z = data[tangent_idx + 2];
    } else {
      vert.tangent = vec3(0, 1, 0);
    }
  }
}

std::vector<float> PrimitiveModelBuilder::WireCubeVertexData() {
  float width = cube.width / 2;
  float height = cube.height / 2;
  float depth = cube.depth / 2;

  vec3 v0(-width, height, -depth);
  vec3 v1(width, height, -depth);
  vec3 v2(-width, height, depth);
  vec3 v3(width, height, depth);

  vec3 v4(-width, -height, -depth);
  vec3 v5(width, -height, -depth);
  vec3 v6(-width, -height, depth);
  vec3 v7(width, -height, depth);

  vector<vec3> pos_list(8);
  pos_list[0] = v0;
  pos_list[1] = v1;
  pos_list[2] = v2;
  pos_list[3] = v3;
  pos_list[4] = v4;
  pos_list[5] = v5;
  pos_list[6] = v6;
  pos_list[7] = v7;

  vector<float> vert_data;
  for(size_t i = 0 ; i < pos_list.size() ; i++) {
    const vec3 &pos = pos_list[i];
    vert_data.push_back(pos[0]);
    vert_data.push_back(pos[1]);
    vert_data.push_back(pos[2]);
  }
  return vert_data;
}

IndexListType PrimitiveModelBuilder::WireCubeIndexList() {
  IndexListType index_list;
  //GL_LINES용 index list
  const GLushort indexList[] = {
    0,1,	1,3,	2,3,	0,2,
    4,5,	5,7,	6,7,	4,6,
    0,4,	1,5,	2,6,	3,7
  };
  for(size_t i = 0 ; i < sizeof(indexList) / sizeof(GLushort) ; i++) {
    index_list.push_back(indexList[i]);
  }
  return index_list;
}

std::vector<float> PrimitiveModelBuilder::WireSphereVertexData() {
  float radius = sphere.radius;
  int slices = sphere.slices;
  int stacks = sphere.stacks;

  //pos(x,y,z)만 저장
  vector<float> vertex_data;
  
  //사용될 vertex list 생성
  std::vector<vec3> tmp_vertex_list;
  for(int i = 0 ; i < stacks ; i++) {
    double yAngle = (kPi / stacks * i) - kPiOver2;
    float y = static_cast<float>(sin(yAngle));

    for(int j = 0 ; j < slices ; j++) {
      double zxAngle = (2.0 * kPi / slices) * j;
      float x = static_cast<float>(cos(yAngle) * cos(zxAngle));
      float z = static_cast<float>(cos(yAngle) * sin(zxAngle));

      vec3 pos(x, y, z);
      tmp_vertex_list.push_back(pos);
    }
  }
  //북극점(맨위)
  vec3 top(0, 1, 0);
  tmp_vertex_list.push_back(top);

  //vertex 위치정보+잡탕으로 진짜 vertex list생성
  vector<vec3>::iterator it;
  for(it = tmp_vertex_list.begin() ; it != tmp_vertex_list.end() ; it++) {
    const vec3 &pos = *it;
    vertex_data.push_back(pos[0]);
    vertex_data.push_back(pos[1]);
    vertex_data.push_back(pos[2]);
  }
  return vertex_data;
}

IndexListType PrimitiveModelBuilder::WireSphereIndexList() {
  float radius = sphere.radius;
  int slices = sphere.slices;
  int stacks = sphere.stacks;

  IndexListType index_list;
  for(int i = 0 ; i < stacks ; i++) {
    //stack 라인 구성
    int start = i * slices;
    for(int j = 0 ; j < slices ; j++) {
      GLushort a = start + j;
      GLushort b = a + 1;
      if(j == slices-1) {
        b = start;
      }
      index_list.push_back(a);
      index_list.push_back(b);
    }
  }
  
  for(int i = 0 ; i < slices ; i++) {
    //slice 라인 구성
    //맨 꼭데기 지점은 몇번쨰 vertex인가
    const GLushort topIndex = stacks * slices;
    for(int j = 0 ; j < stacks ; j++) {
      GLushort a = i + (j*slices);
      GLushort b = (a + slices);
      if(b >= slices*stacks) {
        b = topIndex;
      }
      index_list.push_back(a);
      index_list.push_back(b);
    }
  }
  return index_list;
}

}
