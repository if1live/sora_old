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
#include "primitive_model.h"
#include "template_lib.h"
#include "math_helper.h"

#if SR_USE_PCH == 0
#include "gl_inc.h"
#endif

#include "teapot.h"

using namespace std;

namespace sora {;
struct PrimitiveModelImpl {
  PrimitiveModelImpl(int size) {
    vert_list_group.resize(size);
    index_list_group.resize(size);
    mode_group.resize(size);
  }  
  std::vector<VertexListType> vert_list_group;
  std::vector<IndexListType> index_list_group;
  std::vector<GLenum> mode_group;
};

PrimitiveModel::PrimitiveModel() : impl(NULL) {
}
PrimitiveModel::PrimitiveModel(const PrimitiveModel &o) {
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(o.impl->index_list_group.size());
  impl->index_list_group = o.impl->index_list_group;
  impl->vert_list_group = o.impl->vert_list_group;
  impl->mode_group = o.impl->mode_group;
}
PrimitiveModel& PrimitiveModel::operator=(const PrimitiveModel &o) {
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(o.impl->index_list_group.size());
  impl->index_list_group = o.impl->index_list_group;
  impl->vert_list_group = o.impl->vert_list_group;
  impl->mode_group = o.impl->mode_group;
  return *this;
}

PrimitiveModel::~PrimitiveModel() {
  SafeDelete(impl);
}

std::vector<DrawCommand> PrimitiveModel::GetDrawCmdList() const {
  vector<DrawCommand> cmd_list;
  //draw primitive model
  for(int i = 0 ; i < Count() ; i++) {
    DrawCommand draw_cmd;
    draw_cmd.vert_ptr = vertex_list(i);
    draw_cmd.index_ptr= index_list(i);
    draw_cmd.index_count = index_count(i);
    draw_cmd.draw_mode = draw_mode(i);
    draw_cmd.index_type = GL_UNSIGNED_SHORT;

    cmd_list.push_back(draw_cmd);
  }
  return cmd_list;
}

int PrimitiveModel::Count() const {
  return impl->mode_group.size();
}
const Vertex* PrimitiveModel::vertex_list(int idx) const {
  SR_ASSERT(idx >= 0 && idx < Count());
  const VertexListType &vert_list = impl->vert_list_group[idx];
  if(vert_list.empty() == false) {
    return &vert_list[0];
  } else {
    //0번인덱스를 대표값으로 쓰자
    const VertexListType &vert_list = impl->vert_list_group[0];  
    return &vert_list[0];
  }
}
const void *PrimitiveModel::index_list(int idx) const {
  SR_ASSERT(idx >= 0 && idx < Count());
  return &impl->index_list_group[idx].at(0);
}
int PrimitiveModel::vertex_count(int idx) const {
  SR_ASSERT(idx >= 0 && idx < Count());
  return impl->vert_list_group[idx].size();
}
int PrimitiveModel::index_count(int idx) const {
  SR_ASSERT(idx >= 0 && idx < Count());
  return impl->index_list_group[idx].size();
}
GLenum PrimitiveModel::draw_mode(int idx) const {
  SR_ASSERT(idx >= 0 && idx < Count());
  return impl->mode_group[idx];
}

void PrimitiveModel::WireCube(float width, float height, float depth) {
  SR_ASSERT(width > 0 && height > 0 && depth > 0);
  width /= 2;
  height /= 2;
  depth /= 2;

  //wirecube는 1개의 메시로 표현 가능
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(1);

  VertexListType &vert_list = impl->vert_list_group[0];
  IndexListType &index_list = impl->index_list_group[0];
  impl->mode_group[0] = GL_LINES;

  //top
  //0 1
  //2 3
  //bottom
  //4 5
  //6 7

  //0
  Vec3f v0(-width, height, -depth);
  Vec3f v1(width, height, -depth);
  Vec3f v2(-width, height, depth);
  Vec3f v3(width, height, depth);

  Vec3f v4(-width, -height, -depth);
  Vec3f v5(width, -height, -depth);
  Vec3f v6(-width, -height, depth);
  Vec3f v7(width, -height, depth);

  //선으로 구성되는거니까 texture, normal은 신경안써도 될듯?
  Vertex vertex0; vertex0.pos = v0;
  Vertex vertex1; vertex1.pos = v1;
  Vertex vertex2; vertex2.pos = v2;
  Vertex vertex3; vertex3.pos = v3;
  Vertex vertex4; vertex4.pos = v4;
  Vertex vertex5; vertex5.pos = v5;
  Vertex vertex6; vertex6.pos = v6;
  Vertex vertex7; vertex7.pos = v7;

  vert_list.push_back(vertex0);
  vert_list.push_back(vertex1);
  vert_list.push_back(vertex2);
  vert_list.push_back(vertex3);
  vert_list.push_back(vertex4);
  vert_list.push_back(vertex5);
  vert_list.push_back(vertex6);
  vert_list.push_back(vertex7);

  //GL_LINES용 index list
  const GLushort indexList[] = {
    0,1,	1,3,	2,3,	0,2,
    4,5,	5,7,	6,7,	4,6,
    0,4,	1,5,	2,6,	3,7
  };
  for(size_t i = 0 ; i < sizeof(indexList) / sizeof(GLushort) ; i++) {
    index_list.push_back(indexList[i]);
  }
}

void PrimitiveModel::SolidCube(float width, float height, float depth, bool discreate_normal) {
  SR_ASSERT(width > 0 && height > 0 && depth > 0);
  width = width/2;
  height = height/2;
  depth = depth/2;

  //solid cube는 1개의 메시로 표현 가능
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(1);

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

    Vec3f normal(0, 0, 1);

    Vec2f texCoord1(0, 0);	Vec3f vertex1(-width, -height, depth);
    Vec2f texCoord2(1, 0);	Vec3f vertex2( width, -height, depth);
    Vec2f texCoord3(1, 1);	Vec3f vertex3( width,  height, depth);
    Vec2f texCoord4(0, 1);	Vec3f vertex4(-width,  height, depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;
    //set normal
    if(discreate_normal) {
      v1.normal = normal;
      v2.normal = normal;
      v3.normal = normal;
      v4.normal = normal;
    } else {
      v1.normal = (vertex1 - Vec3f(0, 0, 0)).Normalize();
      v2.normal = (vertex2 - Vec3f(0, 0, 0)).Normalize();
      v3.normal = (vertex3 - Vec3f(0, 0, 0)).Normalize();
      v4.normal = (vertex4 - Vec3f(0, 0, 0)).Normalize();
    }

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
    Vec3f normal(0, 0, -1);

    Vec2f texCoord1(1, 0);	Vec3f vertex1(-width, -height, -depth);
    Vec2f texCoord2(1, 1);	Vec3f vertex2(-width,  height, -depth);
    Vec2f texCoord3(0, 1);	Vec3f vertex3( width,  height, -depth);
    Vec2f texCoord4(0, 0);	Vec3f vertex4( width, -height, -depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;
    //set normal
    if(discreate_normal) {
      v1.normal = normal;
      v2.normal = normal;
      v3.normal = normal;
      v4.normal = normal;
    } else {
      v1.normal = (vertex1 - Vec3f(0, 0, 0)).Normalize();
      v2.normal = (vertex2 - Vec3f(0, 0, 0)).Normalize();
      v3.normal = (vertex3 - Vec3f(0, 0, 0)).Normalize();
      v4.normal = (vertex4 - Vec3f(0, 0, 0)).Normalize();
    }

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
    Vec3f normal(0, 1, 0);

    Vec2f texCoord1(0, 1);	Vec3f vertex1(-width, height, -depth);
    Vec2f texCoord2(0, 0);	Vec3f vertex2(-width, height,  depth);
    Vec2f texCoord3(1, 0);	Vec3f vertex3( width, height,  depth);
    Vec2f texCoord4(1, 1);	Vec3f vertex4( width, height, -depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;
    //set normal
    if(discreate_normal) {
      v1.normal = normal;
      v2.normal = normal;
      v3.normal = normal;
      v4.normal = normal;
    } else {
      v1.normal = (vertex1 - Vec3f(0, 0, 0)).Normalize();
      v2.normal = (vertex2 - Vec3f(0, 0, 0)).Normalize();
      v3.normal = (vertex3 - Vec3f(0, 0, 0)).Normalize();
      v4.normal = (vertex4 - Vec3f(0, 0, 0)).Normalize();
    }

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
    Vec3f normal(0, -1, 0);

    Vec2f texCoord1(1, 1);	Vec3f vertex1(-width, -height, -depth);
    Vec2f texCoord2(0, 1);	Vec3f vertex2( width, -height, -depth);
    Vec2f texCoord3(0, 0);	Vec3f vertex3( width, -height,  depth);
    Vec2f texCoord4(1, 0);	Vec3f vertex4(-width, -height,  depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;
    //set normal
    if(discreate_normal) {
      v1.normal = normal;
      v2.normal = normal;
      v3.normal = normal;
      v4.normal = normal;
    } else {
      v1.normal = (vertex1 - Vec3f(0, 0, 0)).Normalize();
      v2.normal = (vertex2 - Vec3f(0, 0, 0)).Normalize();
      v3.normal = (vertex3 - Vec3f(0, 0, 0)).Normalize();
      v4.normal = (vertex4 - Vec3f(0, 0, 0)).Normalize();
    }

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
    Vec3f normal(1, 0, 0);

    Vec2f texCoord1(1, 0);	Vec3f vertex1(width, -height, -depth);
    Vec2f texCoord2(1, 1);	Vec3f vertex2(width,  height, -depth);
    Vec2f texCoord3(0, 1);	Vec3f vertex3(width,  height,  depth);
    Vec2f texCoord4(0, 0);	Vec3f vertex4(width, -height,  depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;
    //set normal
    if(discreate_normal) {
      v1.normal = normal;
      v2.normal = normal;
      v3.normal = normal;
      v4.normal = normal;
    } else {
      v1.normal = (vertex1 - Vec3f(0, 0, 0)).Normalize();
      v2.normal = (vertex2 - Vec3f(0, 0, 0)).Normalize();
      v3.normal = (vertex3 - Vec3f(0, 0, 0)).Normalize();
      v4.normal = (vertex4 - Vec3f(0, 0, 0)).Normalize();
    }

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
    Vec3f normal(-1, 0, 0);

    Vec2f texCoord1(0, 0);	Vec3f vertex1(-width, -height, -depth);
    Vec2f texCoord2(1, 0);	Vec3f vertex2(-width, -height,  depth);
    Vec2f texCoord3(1, 1);	Vec3f vertex3(-width,  height,  depth);
    Vec2f texCoord4(0, 1);	Vec3f vertex4(-width,  height, -depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;
    //set normal
    if(discreate_normal) {
      v1.normal = normal;
      v2.normal = normal;
      v3.normal = normal;
      v4.normal = normal;
    } else {
      v1.normal = (vertex1 - Vec3f(0, 0, 0)).Normalize();
      v2.normal = (vertex2 - Vec3f(0, 0, 0)).Normalize();
      v3.normal = (vertex3 - Vec3f(0, 0, 0)).Normalize();
      v4.normal = (vertex4 - Vec3f(0, 0, 0)).Normalize();
    }

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
}
void PrimitiveModel::WireSphere(float radius, int slices, int stacks) {
  SR_ASSERT(radius > 0);
  SR_ASSERT(slices > 0);
  SR_ASSERT(stacks > 0);

  //use one mesh
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(1);

  VertexListType &vert_list = impl->vert_list_group[0];
  IndexListType &index_list = impl->index_list_group[0];
  impl->mode_group[0] = GL_LINES;

  //사용될 vertex list 생성
  std::vector<Vec3f> tmp_vertex_list;
  for(int i = 0 ; i < stacks ; i++) {
    double yAngle = (kPi / stacks * i) - kPiOver2;
    float y = static_cast<float>(sin(yAngle));

    for(int j = 0 ; j < slices ; j++) {
      double zxAngle = (2.0 * kPi / slices) * j;
      float x = static_cast<float>(cos(yAngle) * cos(zxAngle));
      float z = static_cast<float>(cos(yAngle) * sin(zxAngle));

      Vec3f pos(x, y, z);
      tmp_vertex_list.push_back(pos);
    }
  }
  //북극점(맨위)
  Vec3f top(0, 1, 0);
  tmp_vertex_list.push_back(top);

  //선으로 구성되니까 해당항목은 없어도 별로 티가 안난다

  //vertex 위치정보+잡탕으로 진짜 vertex list생성
  vector<Vec3f>::iterator it;
  for(it = tmp_vertex_list.begin() ; it != tmp_vertex_list.end() ; it++) {
    const Vec3f &pos = *it;
    Vertex vertex;
    vertex.normal = pos;
    vertex.pos = pos * radius;
    vert_list.push_back(vertex);
  }

  //index list
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
    const GLushort topIndex = vert_list.size()-1;
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
}
void PrimitiveModel::SolidSphere(float radius, int slices, int stacks) {
  SR_ASSERT(radius > 0);
  SR_ASSERT(slices > 0);
  SR_ASSERT(stacks > 0);

  //use one mesh
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(1);
  VertexListType &vert_list = impl->vert_list_group[0];
  IndexListType &index_list = impl->index_list_group[0];
  impl->mode_group[0] = GL_TRIANGLES;

  //원을 구성하는 포인트 리스트 + 텍스쳐 좌표 동시 계산
  vector<Vec3f> posList;
  vector<Vec2f> texCoordList;
  for(int i = 0 ; i <= stacks ; i++) {
    double yAngle = (kPi / stacks * i) - kPiOver2;
    float y = static_cast<float>(sin(yAngle));
    for(int j = 0 ; j <= slices ; j++) {
      float zxAngle = static_cast<float>((2.0 * kPi / slices) * j);
      float x = static_cast<float>(cos(yAngle) * cos(zxAngle));
      float z = static_cast<float>(cos(yAngle) * sin(zxAngle));

      Vec3f pos(x, y, z);
      posList.push_back(pos);

      //tex 좌표 계산
      float s = 1.0f / (slices) * j;
      float t = 1.0f / (stacks) * i;
      Vec2f texCoord(s, t);
      texCoordList.push_back(texCoord);
    }
  }

  //normal vector = 반지름1인 pso

  //vertex 구성
  for(std::size_t i = 0 ; i < posList.size() ; i++) {
    const Vec3f &pos = posList[i];
    const Vec2f &texcoord = texCoordList[i];

    Vertex vert;
    vert.pos = pos * radius;
    vert.normal = pos;
    vert.texcoord = texcoord;
    vert_list.push_back(vert);
  }

  for(int i = 0 ; i < stacks ; i++) {
    GLushort start = i * (slices+1);
    for(int j = 0 ; j < slices ; j++) {
      //d c
      //b a
      GLushort a = j + start;
      GLushort b = a + 1;
      GLushort c = a + slices + 1;
      GLushort d = c + 1;


      index_list.push_back(a);
      index_list.push_back(c);
      index_list.push_back(b);

      index_list.push_back(c);
      index_list.push_back(d);
      index_list.push_back(b);
    }
  }
}
void PrimitiveModel::WireCone(float base, float height, int slices, int stacks) {
  SR_ASSERT(base > 0);
  SR_ASSERT(height > 0);
  SR_ASSERT(slices > 0);
  SR_ASSERT(stacks > 0);

  //밑면/옆면을 다른 메시로 처리하자
  //vertex List는 공유하고 index만 다르게 하자
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(2);
  VertexListType &vert_list = impl->vert_list_group[0];
  IndexListType &stackindex_list = impl->index_list_group[0];
  IndexListType &sliceindex_list = impl->index_list_group[1];
  impl->mode_group[0] = GL_LINES;
  impl->mode_group[1] = GL_LINES;

  //밑면
  vector<Vec3f> posList; 
  for(int j = 0 ; j < stacks ; j++) {
    float y = (1.0f / (float)stacks) * j;
    for(int i = 0 ; i < slices ; i++) {
      double angleDt = 2.0 * kPi / slices;
      float radius = (1.0f / stacks) * (stacks-j);
      float x = static_cast<float>(cos(angleDt * i) * radius);
      float z = static_cast<float>(sin(angleDt * i) * radius);

      Vec3f pos(x, y, z);
      posList.push_back(pos);
    }
  }
  //최상위 꼭지점은 따로 넣는다
  Vec3f toppos(0, 1, 0);
  posList.push_back(toppos);


  //vertex 구성
  //vec2 texCoord(0, 0);
  //vec3 normal(0, 0, 0);
  for(std::size_t i = 0 ; i < posList.size() ; i++) {
    Vec3f &pos = posList[i];
    pos.x *= base;
    pos.y *= height;
    pos.z *= base;
    
    Vertex v;
    v.pos = pos;
    vert_list.push_back(v);
  }

  //index for bottom;
  for(int j = 0 ; j < stacks ; j++) {
    int startIndex = -1;
    for(int i = 0 ; i < slices ; i++) {
      if(i == 0) {
        startIndex = j*slices;
      }
      GLushort a = startIndex + i;
      GLushort b = ((a + 1) % slices) + startIndex;
      stackindex_list.push_back(a);
      stackindex_list.push_back(b);
    }
  }

  //꼭지점에서 base로 내려오는 직선 긋기
  GLushort topVertexIndex = vert_list.size()-1;
  for(int i = 0 ; i < slices ; i++) {
    sliceindex_list.push_back(i);
    sliceindex_list.push_back(topVertexIndex);
  }
}
void PrimitiveModel::SolidCone(float base, float height, int slices, int stacks) {
  SR_ASSERT(base > 0);
  SR_ASSERT(height > 0);
  SR_ASSERT(slices > 2);
  SR_ASSERT(stacks > 0);

  //밑면/옆면을 다른 메시로 처리하자
  //vertex List는 공유하고 index만 다르게 하자
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(2);
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
      Vec3f pos(x, y, z);
      Vec3f normal = pos.Normalize();

      float texCoordS = (1.0f / slices) * i;
      float texCoordT = (1.0f / stacks) * j;
      assert(texCoordS >= 0 && texCoordS <= 1 && texCoordT >= 0 && texCoordT <= 1);
      Vec2f texCoord(texCoordS, texCoordT);

      Vertex v;
      v.pos = pos;
      v.texcoord = texCoord;
      v.normal = normal;
      vert_list.push_back(v);
    }
  }

  //밑바닥용 vertex pos는 다시 계산한다. normal+tex가 옆면과 달라야되기 떄문에 추가로 필요하다
  Vec3f bottomNormal(0, -1, 0);
  const int bottomVertexStartIndex = vert_list.size();	//bottom vertex 정보가 시작되는 인덱스
  for(int i = 0 ; i < slices ; i++) {
    float angleDt = static_cast<float>(2.0 * kPi / slices);
    float x = cos(angleDt * i) * base;
    float z = sin(angleDt * i) * base;
    Vec3f pos(x, 0, z);
    //texcoord 계산
    Vec2f texCoord((cos(angleDt * i) + 1) / 2, (sin(angleDt *i) + 1) / 2);

    Vertex v;
    v.pos = pos;
    v.texcoord = texCoord;
    v.normal = bottomNormal;
    vert_list.push_back(v);
  }

  //밑바닥 중심점
  Vec3f bottompos(0, 0, 0);
  Vec2f bottomCenterTexCoord(0.5, 0.5);
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

  //1개의 메시로 구성
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(1);
  VertexListType &vert_list = impl->vert_list_group[0];
  IndexListType &index_list = impl->index_list_group[0];
  impl->mode_group[0] = GL_LINES;

  //원기둥의 높이(z)는 -height/2~height/2로 height를 구성함
  const float angleDt = static_cast<float>(2.0 * kPi / slices);
  for(int j = 0 ; j < 2 ; j++) {
    for(int i = 0 ; i < slices ; i++) {	
      float x = static_cast<float>(cos(angleDt * i) * radius);
      float z = static_cast<float>(sin(angleDt * i) * radius);

      float y = 0;
      if(j == 0) {
        y = height / 2;	//윗면
      } else {
        y = -height / 2;		//아랫면
      }

      Vec3f pos(x, y, z);
      Vertex v; v.pos = pos;
      vert_list.push_back(v);
    }
  }
  //밑점, 윗점 따로 vertex list에 넣기
  Vec3f bottomPos(0, -height/2, 0);
  Vec3f topPos(0, height/2, 0);
  Vertex bottom; bottom.pos = bottomPos;
  Vertex top; top.pos = topPos;
  vert_list.push_back(bottom);
  vert_list.push_back(top);

  //index list구성
  const int topIndex = vert_list.size() - 1;
  const int bottomIndex = vert_list.size() - 2;

  for(int i = 0 ; i < slices ; i++) {
    //옆에서 봤을떄의 얻어낸 정보
    //a b
    //c d
    GLushort a = i;
    GLushort c = i + slices;
    GLushort b = (i + 1) % slices;
    GLushort d = (c + 1) % slices + slices;

    //윗면과 아랫면 선으로 연결하기 + 대각선도 넣기
    index_list.push_back(a);
    index_list.push_back(c);

    index_list.push_back(b);
    index_list.push_back(d);

    index_list.push_back(a);
    index_list.push_back(d);

    //윗면의 테두리 그리기	
    index_list.push_back(a);
    index_list.push_back(b);

    //아랫면의 테두리 그리기
    index_list.push_back(c);
    index_list.push_back(d);

    //아랫면 덮기
    index_list.push_back(c);
    index_list.push_back(bottomIndex);

    //윗면 덮기
    index_list.push_back(a);
    index_list.push_back(topIndex);
  }
}
void PrimitiveModel::SolidCylinder(float radius, float height, int slices) {
  SR_ASSERT(radius > 0);
  SR_ASSERT(height > 0);
  SR_ASSERT(slices >= 4);

  //use 3 mesh
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(3);
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
      Vec2f texCoord;
      if(j == 0) {
        y = height / 2;	//윗면
        texCoord = Vec2f(static_cast<float>(cos(angleDt * i)), 1);
      }	else {
        y = -height / 2;		//아랫면
        texCoord = Vec2f(static_cast<float>(cos(angleDt * i)), 0);
      }

      Vec3f pos(x, y, z);

      //normal 계산
      static Vec3f zero(0, 0, 0);
      Vec3f normal = pos - zero;
      normal.Normalized();

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
    Vec3f bottomPos(0, -height/2, 0);
    Vec2f texCoord(0, 0);
    Vec3f normal(0, -1, 0);
    Vertex bottom;
    bottom.pos = bottomPos;
    bottom.texcoord = texCoord;
    bottom.normal = normal;
    vert_list.push_back(bottom);
  }

  //윗점
  {
    Vec3f topPos(0, height/2, 0);
    Vec2f texCoord(0, 0);
    Vec3f normal(0, 1, 0);
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
void PrimitiveModel::WireAxis(float size) {
  SR_ASSERT(size > 0);

  //asiz는 1개의 메시로 표현 가능
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(1);

  VertexListType &vert_list = impl->vert_list_group[0];
  IndexListType &index_list = impl->index_list_group[0];
  impl->mode_group[0] = GL_LINES;

  //vertex list 생성
  Vec3f xPos(size, 0, 0);
  Vec3f yPos(0, size, 0);
  Vec3f zPos(0, 0, size);
  Vec3f zero(0, 0, 0);

  Vec4ub red(255, 0, 0, 255);
  Vec4ub green(0, 255, 0, 255);
  Vec4ub blue(0, 0, 255, 255);

  //x axis - r
  Vertex vx0; vx0.pos = zero; vx0.color = red;
  Vertex vx1; vx1.pos = xPos; vx1.color = red;
  vert_list.push_back(vx0);
  vert_list.push_back(vx1);

  //y axis - g
  Vertex vy0; vy0.pos = zero; vy0.color = green;
  Vertex vy1; vy1.pos = yPos; vy1.color = green;
  vert_list.push_back(vy0);
  vert_list.push_back(vy1);

  //z axis - b
  Vertex vz0; vz0.pos = zero; vz0.color = blue;
  Vertex vz1; vz1.pos = zPos; vz1.color = blue;
  vert_list.push_back(vz0);
  vert_list.push_back(vz1);

  //create index list
  for(GLushort i = 0 ; i < 6 ; i++) {
    index_list.push_back(i);
  }
}


void PrimitiveModel::CalcTeapotSize() {
  if(teapot_created_ == true) {
    return;
  }

  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_VERTEX ; i++) {
    float x = teapotVertices[i*3 + 0];
    float y = teapotVertices[i*3 + 1];
    float z = teapotVertices[i*3 + 2];

    if(x < teapot_min_x_) {
      teapot_min_x_ = x;
    } else if(x > teapot_max_x_) {
      teapot_max_x_ = x;
    }
    if(y < teapot_min_y_) {
      teapot_min_y_ = y;
    } else if(y > teapot_max_y_) {
      teapot_max_y_ = y;
    }
    if(z < teapot_min_z_) {
      teapot_min_z_ = z;
    } else if(z > teapot_max_z_) {
      teapot_max_z_ = z;
    }
  }
}
//teapot
float PrimitiveModel::teapot_min_x_ = 1000000;
float PrimitiveModel::teapot_max_x_ = -1000000;
float PrimitiveModel::teapot_min_y_ = 1000000;
float PrimitiveModel::teapot_max_y_ = -1000000;
float PrimitiveModel::teapot_min_z_ = 1000000;
float PrimitiveModel::teapot_max_z_ = -1000000;
bool PrimitiveModel::teapot_created_ = false;

void PrimitiveModel::WireTeapot(float size) {
  CalcTeapotSize();

  SR_ASSERT(size > 0);
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(1);

  VertexListType &vert_list = impl->vert_list_group[0];
  IndexListType &index_list = impl->index_list_group[0];
  impl->mode_group[0] = GL_LINES;

  //get bounding box to resize
  float orig_width = teapot_max_x_ - teapot_min_x_;
  float orig_height = teapot_max_y_ - teapot_min_y_;
  float orig_depth = teapot_max_z_ - teapot_min_z_;
  float max_size = orig_width > orig_height ? orig_width : orig_height;
  if(max_size < orig_depth) {
    max_size = orig_depth;
  }
  float scale = size / max_size;

  vert_list.resize(NUM_TEAPOT_OBJECT_VERTEX);
  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_VERTEX ; i++) {
    Vertex &vert = vert_list[i];
    vert.pos.x = teapotVertices[i * 3 + 0] * scale;
    vert.pos.y = teapotVertices[i * 3 + 1] * scale;
    vert.pos.z = teapotVertices[i * 3 + 2] * scale;

    vert.texcoord.x = teapotTexCoords[i*2 + 0];
    vert.texcoord.y = teapotTexCoords[i*2 + 1];

    vert.normal.x = teapotNormals[i*3 + 0];
    vert.normal.y = teapotNormals[i*3 + 1];
    vert.normal.z = teapotNormals[i*3 + 2];
  }

  index_list.reserve(NUM_TEAPOT_OBJECT_INDEX * 2);
  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_INDEX / 3 ; i++) {
    int idx1 = teapotIndices[i*3 + 0];
    int idx2 = teapotIndices[i*3 + 1];
    int idx3 = teapotIndices[i*3 + 2];

    index_list.push_back(idx1);
    index_list.push_back(idx2);

    index_list.push_back(idx2);
    index_list.push_back(idx3);

    index_list.push_back(idx3);
    index_list.push_back(idx1);
  }
}
void PrimitiveModel::SolidTeapot(float size) {
  CalcTeapotSize();

  SR_ASSERT(size > 0);
  SafeDelete(impl);
  impl = new PrimitiveModelImpl(1);

  VertexListType &vert_list = impl->vert_list_group[0];
  IndexListType &index_list = impl->index_list_group[0];
  impl->mode_group[0] = GL_TRIANGLES;

  float orig_width = teapot_max_x_ - teapot_min_x_;
  float orig_height = teapot_max_y_ - teapot_min_y_;
  float orig_depth = teapot_max_z_ - teapot_min_z_;
  float max_size = orig_width > orig_height ? orig_width : orig_height;
  if(max_size < orig_depth) {
    max_size = orig_depth;
  }
  float scale = size / max_size;

  vert_list.resize(NUM_TEAPOT_OBJECT_VERTEX);
  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_VERTEX ; i++) {
    Vertex &vert = vert_list[i];
    vert.pos.x = teapotVertices[i * 3 + 0] * scale;
    vert.pos.y = teapotVertices[i * 3 + 1] * scale;
    vert.pos.z = teapotVertices[i * 3 + 2] * scale;

    vert.texcoord.x = teapotTexCoords[i*2 + 0];
    vert.texcoord.y = teapotTexCoords[i*2 + 1];

    vert.normal.x = teapotNormals[i*3 + 0];
    vert.normal.y = teapotNormals[i*3 + 1];
    vert.normal.z = teapotNormals[i*3 + 2];
  }

  index_list.resize(NUM_TEAPOT_OBJECT_INDEX);
  memcpy(&index_list[0], teapotIndices, sizeof(unsigned short) * NUM_TEAPOT_OBJECT_INDEX);
}
}

