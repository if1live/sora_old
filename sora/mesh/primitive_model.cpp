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
#include "sora_stdafx.h"
#include "mesh/primitive_model.h"
#include "core/template_lib.h"
#include "core/math_helper.h"
#include "mesh_helper.h"

#if SR_USE_PCH == 0
#include "gl_inc.h"
#endif

#include "teapot.h"

using namespace std;
using namespace glm;

#if 0
namespace sora {;
void PrimitiveModel::SolidCube(float width, float height, float depth) {
  SR_ASSERT(width > 0 && height > 0 && depth > 0);
  width = width/2;
  height = height/2;
  depth = depth/2;

  //solid cube는 1개의 메시로 표현 가능
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(1, true);

  VertexListType &vert_list = impl->vert_list_group[0];
  IndexListType &index_list = impl->index_list_group[0];
  impl->mode_group[0] = GL_TRIANGLES;

  //construct vertex list
  //GL_TRIANGLES용 index list도 같이 구성한다
  //ccw base

  //normal
  {
    // Front Face
    int baseIndex = vert_list.size();

    vec2 texCoord1(0, 0);	vec3 vertex1(-width, -height, depth);
    vec2 texCoord2(1, 0);	vec3 vertex2( width, -height, depth);
    vec2 texCoord3(1, 1);	vec3 vertex3( width,  height, depth);
    vec2 texCoord4(0, 1);	vec3 vertex4(-width,  height, depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;

    vert_list.push_back(v1);
    vert_list.push_back(v2);
    vert_list.push_back(v3);
    vert_list.push_back(v4);

    //add index
    index_list.push_back(baseIndex + 0);
    index_list.push_back(baseIndex + 1);
    index_list.push_back(baseIndex + 2);

    index_list.push_back(baseIndex + 0);
    index_list.push_back(baseIndex + 2);
    index_list.push_back(baseIndex + 3);
  }
  {
    // Back Face
    int baseIndex = vert_list.size();

    vec2 texCoord1(1, 0);	vec3 vertex1(-width, -height, -depth);
    vec2 texCoord2(1, 1);	vec3 vertex2(-width,  height, -depth);
    vec2 texCoord3(0, 1);	vec3 vertex3( width,  height, -depth);
    vec2 texCoord4(0, 0);	vec3 vertex4( width, -height, -depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;

    vert_list.push_back(v1);
    vert_list.push_back(v2);
    vert_list.push_back(v3);
    vert_list.push_back(v4);

    //add index
    index_list.push_back(baseIndex + 0);
    index_list.push_back(baseIndex + 1);
    index_list.push_back(baseIndex + 2);

    index_list.push_back(baseIndex + 0);
    index_list.push_back(baseIndex + 2);
    index_list.push_back(baseIndex + 3);
  }

  {
    // Top Face
    int baseIndex = vert_list.size();

    vec2 texCoord1(0, 1);	vec3 vertex1(-width, height, -depth);
    vec2 texCoord2(0, 0);	vec3 vertex2(-width, height,  depth);
    vec2 texCoord3(1, 0);	vec3 vertex3( width, height,  depth);
    vec2 texCoord4(1, 1);	vec3 vertex4( width, height, -depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;

    vert_list.push_back(v1);
    vert_list.push_back(v2);
    vert_list.push_back(v3);
    vert_list.push_back(v4);

    //add index
    index_list.push_back(baseIndex + 0);
    index_list.push_back(baseIndex + 1);
    index_list.push_back(baseIndex + 2);

    index_list.push_back(baseIndex + 0);
    index_list.push_back(baseIndex + 2);
    index_list.push_back(baseIndex + 3);
  }

  {
    // Bottom Face
    int baseIndex = vert_list.size();

    vec2 texCoord1(1, 1);	vec3 vertex1(-width, -height, -depth);
    vec2 texCoord2(0, 1);	vec3 vertex2( width, -height, -depth);
    vec2 texCoord3(0, 0);	vec3 vertex3( width, -height,  depth);
    vec2 texCoord4(1, 0);	vec3 vertex4(-width, -height,  depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;

    vert_list.push_back(v1);
    vert_list.push_back(v2);
    vert_list.push_back(v3);
    vert_list.push_back(v4);

    //add index
    index_list.push_back(baseIndex + 0);
    index_list.push_back(baseIndex + 1);
    index_list.push_back(baseIndex + 2);

    index_list.push_back(baseIndex + 0);
    index_list.push_back(baseIndex + 2);
    index_list.push_back(baseIndex + 3);
  }

  {
    // Right face
    int baseIndex = vert_list.size();

    vec2 texCoord1(1, 0);	vec3 vertex1(width, -height, -depth);
    vec2 texCoord2(1, 1);	vec3 vertex2(width,  height, -depth);
    vec2 texCoord3(0, 1);	vec3 vertex3(width,  height,  depth);
    vec2 texCoord4(0, 0);	vec3 vertex4(width, -height,  depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;

    vert_list.push_back(v1);
    vert_list.push_back(v2);
    vert_list.push_back(v3);
    vert_list.push_back(v4);

    //add index
    index_list.push_back(baseIndex + 0);
    index_list.push_back(baseIndex + 1);
    index_list.push_back(baseIndex + 2);

    index_list.push_back(baseIndex + 0);
    index_list.push_back(baseIndex + 2);
    index_list.push_back(baseIndex + 3);
  }

  {
    // Left Face
    int baseIndex = vert_list.size();

    vec2 texCoord1(0, 0);	vec3 vertex1(-width, -height, -depth);
    vec2 texCoord2(1, 0);	vec3 vertex2(-width, -height,  depth);
    vec2 texCoord3(1, 1);	vec3 vertex3(-width,  height,  depth);
    vec2 texCoord4(0, 1);	vec3 vertex4(-width,  height, -depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;

    vert_list.push_back(v1);
    vert_list.push_back(v2);
    vert_list.push_back(v3);
    vert_list.push_back(v4);

    //add index
    index_list.push_back(baseIndex + 0);
    index_list.push_back(baseIndex + 1);
    index_list.push_back(baseIndex + 2);

    index_list.push_back(baseIndex + 0);
    index_list.push_back(baseIndex + 2);
    index_list.push_back(baseIndex + 3);
  }

  //auto build normal
  MeshHelper<Vertex>::BuildNormal(vert_list, index_list);
}
void PrimitiveModel::SolidCone(float base, float height, int slices, int stacks) {
  SR_ASSERT(base > 0);
  SR_ASSERT(height > 0);
  SR_ASSERT(slices > 2);
  SR_ASSERT(stacks > 0);

  //밑면/옆면을 다른 메시로 처리하자
  //vertex List는 공유하고 index만 다르게 하자
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(2, true);
  VertexListType &vert_list = impl->vert_list_group[0];
  IndexListType &sideindex_list = impl->index_list_group[0];
  IndexListType &bottomindex_list = impl->index_list_group[1];
  impl->mode_group[0] = GL_TRIANGLES;
  impl->mode_group[1] = GL_TRIANGLE_FAN;

  //점 목록 찍기. 옆면용 + vertex list 구성
  for(int j = 0 ; j <= stacks ; j++) {
    float y = (height / stacks) * j;
    for(int i = 0 ; i <= slices ; i++) {
      float angleDt = static_cast<float>(2.0 * kPi / slices);
      float radius = (1.0f / stacks) * (stacks-j)* base;
      float x = cos(angleDt * i) * radius;
      float z = sin(angleDt * i) * radius;
      vec3 pos(x, y, z);
      vec3 normal = normalize(pos);

      float texCoordS = (1.0f / slices) * i;
      float texCoordT = (1.0f / stacks) * j;
      assert(texCoordS >= 0 && texCoordS <= 1 && texCoordT >= 0 && texCoordT <= 1);
      vec2 texCoord(texCoordS, texCoordT);

      Vertex v;
      v.pos = pos;
      v.texcoord = texCoord;
      v.normal = normal;
      vert_list.push_back(v);
    }
  }

  //밑바닥용 vertex pos는 다시 계산한다. normal+tex가 옆면과 달라야되기 떄문에 추가로 필요하다
  vec3 bottomNormal(0, -1, 0);
  const int bottomVertexStartIndex = vert_list.size();	//bottom vertex 정보가 시작되는 인덱스
  for(int i = 0 ; i < slices ; i++) {
    float angleDt = static_cast<float>(2.0 * kPi / slices);
    float x = cos(angleDt * i) * base;
    float z = sin(angleDt * i) * base;
    vec3 pos(x, 0, z);
    //texcoord 계산
    vec2 texCoord((cos(angleDt * i) + 1) / 2, (sin(angleDt *i) + 1) / 2);

    Vertex v;
    v.pos = pos;
    v.texcoord = texCoord;
    v.normal = bottomNormal;
    vert_list.push_back(v);
  }

  //밑바닥 중심점
  vec3 bottompos(0, 0, 0);
  vec2 bottomCenterTexCoord(0.5, 0.5);
  Vertex bottomCenterVertex;
  bottomCenterVertex.pos = bottompos;
  bottomCenterVertex.texcoord = bottomCenterTexCoord;
  bottomCenterVertex.normal = bottomNormal;
  vert_list.push_back(bottomCenterVertex);

  //index list
  //옆면부터
  for(int i = 0 ; i < stacks ; i++) {
    GLushort start = i * (slices+1);
    for(int j = 0 ; j < slices ; j++) {
      //x-z평면의 좌표계의 방향은 x-y평면과 증가방향이 반대다
      //d c
      //b a
      GLushort a = start + j;
      GLushort b = a + 1;
      GLushort c = a + (slices + 1);
      GLushort d = c + 1;

      sideindex_list.push_back(a);
      sideindex_list.push_back(c);
      sideindex_list.push_back(d);

      sideindex_list.push_back(a);
      sideindex_list.push_back(d);
      sideindex_list.push_back(b);
    }
  }
  //밑면
  //화면의 밑점은 가장 마지막에 넣었음
  GLushort bottomVertexIndex = vert_list.size()-1;
  bottomindex_list.push_back(bottomVertexIndex);
  for(int i = 0 ; i < slices ; i++) {
    bottomindex_list.push_back(i + bottomVertexStartIndex);
  }
  //처음점을 다시 찍어야 원형 완성
  bottomindex_list.push_back(bottomVertexStartIndex);
}
void PrimitiveModel::WireCylinder(float radius, float height, int slices) {
  SR_ASSERT(radius > 0);
  SR_ASSERT(height > 0);
  SR_ASSERT(slices >= 4);

  
}
void PrimitiveModel::SolidCylinder(float radius, float height, int slices) {
  SR_ASSERT(radius > 0);
  SR_ASSERT(height > 0);
  SR_ASSERT(slices >= 4);

  //use 3 mesh
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(3, true);
  VertexListType &vert_list = impl->vert_list_group[0];
  IndexListType &topindex_list = impl->index_list_group[0];
  IndexListType &bottomindex_list = impl->index_list_group[1];
  IndexListType &sideindex_list = impl->index_list_group[2];
  impl->mode_group[0] = GL_TRIANGLE_FAN;
  impl->mode_group[1] = GL_TRIANGLE_FAN;
  impl->mode_group[2] = GL_TRIANGLE_STRIP;

  //원기둥의 높이(z)는 -height/2~height/2로 height를 구성함
  const float angleDt = static_cast<float>(2.0 * kPi / slices);
  for(int j = 0 ; j < 2 ; j++) {
    for(int i = 0 ; i < slices ; i++) {	
      float x = static_cast<float>(cos(angleDt * i) * radius);
      float z = static_cast<float>(sin(angleDt * i) * radius);

      float y = 0;
      vec2 texCoord;
      if(j == 0) {
        y = height / 2;	//윗면
        texCoord = vec2(static_cast<float>(cos(angleDt * i)), 1);
      }	else {
        y = -height / 2;		//아랫면
        texCoord = vec2(static_cast<float>(cos(angleDt * i)), 0);
      }

      vec3 pos(x, y, z);

      //normal 계산
      static vec3 zero(0, 0, 0);
      vec3 normal = pos - zero;
      normal = glm::normalize(normal);

      Vertex v;
      v.pos = pos;
      v.texcoord = texCoord;
      v.normal = normal;
      vert_list.push_back(v);
    }
  }
  //밑점, 윗점 따로 vertex list에 넣기
  //밑점
  {
    vec3 bottomPos(0, -height/2, 0);
    vec2 texCoord(0, 0);
    vec3 normal(0, -1, 0);
    Vertex bottom;
    bottom.pos = bottomPos;
    bottom.texcoord = texCoord;
    bottom.normal = normal;
    vert_list.push_back(bottom);
  }

  //윗점
  {
    vec3 topPos(0, height/2, 0);
    vec2 texCoord(0, 0);
    vec3 normal(0, 1, 0);
    Vertex top;
    top.pos = topPos;
    top.texcoord = texCoord;
    top.normal = normal;
    vert_list.push_back(top);
  }

  //index list구성
  const int topIndex = vert_list.size() - 1;
  const int bottomIndex = vert_list.size() - 2;

  topindex_list.push_back(topIndex);
  bottomindex_list.push_back(bottomIndex);

  for(int i = 0 ; i < slices ; i++) {
    //옆에서 봤을떄의 얻어낸 정보
    //a b
    //c d
    GLushort a = i;
    GLushort c = i + slices;
    GLushort b = (i + 1) % slices;
    GLushort d = (c + 1) % slices + slices;

    //옆면 삼각형 인덱스 생성
    sideindex_list.push_back(a);
    sideindex_list.push_back(c);
    sideindex_list.push_back(d);

    sideindex_list.push_back(a);
    sideindex_list.push_back(d);
    sideindex_list.push_back(b);
  }

  for(int i = 0 ; i <= slices ; i++) {
    //옆에서 봤을떄의 얻어낸 정보
    //a b
    //c d
    GLushort a = (i) % slices;
    GLushort c = a + slices;

    //아랫면 덮기
    bottomindex_list.push_back(c);
  }

  for(int i = slices-1 ; i >= -1 ; i--) {
    //옆에서 봤을떄의 얻어낸 정보
    //a b
    //c d
    GLushort a = (i) % slices;
    if(i == -1) {
      a = slices-1;
    }

    //윗면 덮기, (인덱스 넣는 방향 반대여야 cull통과한다)
    topindex_list.push_back(a);
  }

}
void PrimitiveModel::WirePlane(float half_size, float grid_size) {
  SR_ASSERT(half_size > 0);
  SR_ASSERT(grid_size > 0);

  SafeDelete(impl);
  //평면 + 축 총 2개의 속성으로 구성된다
  impl = new PrimitiveModelImpl(2, false);
  
  {
  }
  {
    //축 부분
    VertexListType &vert_list = impl->vert_list_group[1];
    IndexListType &index_list = impl->index_list_group[1];
    impl->mode_group[0] = GL_LINES;

    vert_list.resize(4);
    //공통 속성 설정
    for(size_t i = 0 ; i < vert_list.size() ; ++i) {
      Vertex &v = vert_list[i];
      v.normal = vec3(0, 1, 0);
      v.texcoord = vec2(0, 0);
    }

    Vertex &left = vert_list[0];
    Vertex &right = vert_list[1];
    Vertex &front = vert_list[2];
    Vertex &back = vert_list[3];

    left.color = vec4ub(255, 0, 0, 255);
    left.pos = vec3(-half_size, 0, 0);
    right.color = vec4ub(255, 0, 0, 255);
    right.pos = vec3(+half_size, 0, 0);

    front.color = vec4ub(0, 255, 0, 255);
    front.pos = vec3(0, 0, +half_size);
    back.color = vec4ub(0, 255, 0, 255);
    back.pos = vec3(0, 0, -half_size);

    //build index list
    index_list.reserve(4);
    index_list.push_back(0);  index_list.push_back(1);
    index_list.push_back(2);  index_list.push_back(3);
  }
}

void PrimitiveModel::SolidPlane(float half_size) {
  SR_ASSERT(half_size > 0);

  SafeDelete(impl);
  impl = new PrimitiveModelImpl(1, false);

  VertexListType &vert_list = impl->vert_list_group[0];
  IndexListType &index_list = impl->index_list_group[0];
  impl->mode_group[0] = GL_TRIANGLES;
  impl->is_solid = true;

  vert_list.resize(4);
  for(size_t i = 0 ; i < vert_list.size() ; i++) {
    Vertex &v = vert_list[i];
    v.color = vec4ub(255, 255, 255, 255);
    v.normal = vec3(0, 1, 0);
  }

  enum {
    kLeftBack,
    kLeftFront,
    kRightBack,
    kRightFront,
  };
  // x scale, z scale, s, t
  float table[][4] = {
    { -1, -1, 0, 0 },
    { -1, +1, 0, 1 },
    { +1, -1, 1, 0 },
    { +1, +1, 1, 1 }
  };
  for(int i = 0 ; i < 4 ; ++i) {
    float x_scale = table[i][0];
    float z_scale = table[i][1];
    float s = table[i][2];
    float t = table[i][3];
    vert_list[i].pos = vec3(x_scale * half_size, 0, z_scale * half_size);
    vert_list[i].texcoord = vec2(s, t);
  }

  index_list.reserve(6);
  index_list.push_back(kLeftBack); 
  index_list.push_back(kLeftFront);  
  index_list.push_back(kRightBack);  
  
  index_list.push_back(kRightBack);
  index_list.push_back(kLeftFront);
  index_list.push_back(kRightFront);
}
}

#endif