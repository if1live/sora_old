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
#include "teapot.h"

using namespace std;
using namespace glm;

namespace sora {;

PrimitiveModelBuilder::PrimitiveModelBuilder(unsigned int flag)
: flag_(flag) {
  cube.width = 1;
  cube.height = 1;
  cube.depth = 1;
}

void PrimitiveModelBuilder::SetCube(float width, float height, float depth) {
  SR_ASSERT(width > 0 && height > 0 && depth > 0);
  //선그리는건 위치정보만 뱉으면 충분하다
  //SR_ASSERT(flag_ == 0);
  cube.width = width;
  cube.height = height;
  cube.depth = depth;
}
void PrimitiveModelBuilder::SetSphere(float radius, int slices, int stacks) {
  SR_ASSERT(radius > 0);
  SR_ASSERT(slices > 0);
  SR_ASSERT(stacks > 0);
  //선그리는건 위치정보만 뱉으면 충분하다
  //SR_ASSERT(flag_ == 0);
  sphere.radius = radius;
  sphere.slices = slices;
  sphere.stacks = stacks;
}

void PrimitiveModelBuilder::SetAxis(float size) {
  SR_ASSERT(size > 0);
  //SR_ASSERT(flag_ == 0 || flag_ == kFlagColor);
  axis.size = size;
}

void PrimitiveModelBuilder::SetPlane(float half_size, float grid_size) {
  SR_ASSERT(half_size > 0);
  SR_ASSERT(grid_size > 0);
  plane.half_size = half_size;
  plane.grid_size = grid_size;
}

void PrimitiveModelBuilder::SetTeapot(float size) {
  SR_ASSERT(size > 0);
  teapot.size = size;
}
void PrimitiveModelBuilder::SetCone(float base, float height, int slices, int stacks) {
  SR_ASSERT(base > 0);
  SR_ASSERT(height > 0);
  SR_ASSERT(slices > 0);
  SR_ASSERT(stacks > 0);
  cone.base = base;
  cone.height = height;
  cone.slices = slices;
  cone.stacks = stacks;
}

void PrimitiveModelBuilder::SetCylinder(float radius, float height, int slices) {
  SR_ASSERT(radius > 0);
  SR_ASSERT(height > 0);
  SR_ASSERT(slices >= 4);
  cylinder.radius = radius;
  cylinder.height = height;
  cylinder.slices = slices;
}

//teapot
float PrimitiveModelBuilder::teapot_min_x_ = 1000000;
float PrimitiveModelBuilder::teapot_max_x_ = -1000000;
float PrimitiveModelBuilder::teapot_min_y_ = 1000000;
float PrimitiveModelBuilder::teapot_max_y_ = -1000000;
float PrimitiveModelBuilder::teapot_min_z_ = 1000000;
float PrimitiveModelBuilder::teapot_max_z_ = -1000000;
bool PrimitiveModelBuilder::teapot_created_ = false;


void PrimitiveModelBuilder::CalcTeapotSize() {
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

void PrimitiveModelBuilder::Append(std::vector<float> &vert_data, const glm::vec2 &v) {
  vert_data.push_back(v.x);
  vert_data.push_back(v.y);
}
void PrimitiveModelBuilder::Append(std::vector<float> &vert_data, const glm::vec3 &v) {
  vert_data.push_back(v.x);
  vert_data.push_back(v.y);
  vert_data.push_back(v.z);
}
void PrimitiveModelBuilder::Append(std::vector<float> &vert_data, const glm::vec4 &v) {
  vert_data.push_back(v.x);
  vert_data.push_back(v.y);
  vert_data.push_back(v.z);
  vert_data.push_back(v.w);
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
    Append(vert_data, pos);
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


  int pos_offset = 0;
  int color_offset = -1;
  int normal_offset = -1;
  int texcoord_offset = -1;
  int tangent_offset = -1;

  unsigned int flag_mask = kFlagColor;
  unsigned int valid_flag = flag_ & flag_mask;
  int rowsize = CalcOffset(valid_flag, &pos_offset, &color_offset, &normal_offset, &texcoord_offset, &tangent_offset);

  //vertex 위치정보+잡탕으로 진짜 vertex list생성
  vector<vec3>::iterator it;
  vertex_data.resize(rowsize * tmp_vertex_list.size());
  int i = 0;
  for(it = tmp_vertex_list.begin() ; it != tmp_vertex_list.end() ; it++, ++i) {
    const vec3 &pos = *it;
    int base_idx = i * rowsize;

    vertex_data[base_idx + pos_offset + 0] = pos.x;
    vertex_data[base_idx + pos_offset + 1] = pos.y;
    vertex_data[base_idx + pos_offset + 2] = pos.z;

    if(color_offset != -1) {
      for(int j = 0 ; j < 4 ; j++) {
        vertex_data[base_idx + color_offset + j] = 255;
      }
    }
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
std::vector<float> PrimitiveModelBuilder::WireAxisVertexData() {
  float size = axis.size;
  vector<float> vert_data;

  //vertex list 생성
  vec3 xPos(size, 0, 0);
  vec3 yPos(0, size, 0);
  vec3 zPos(0, 0, size);
  vec3 zero(0, 0, 0);

  vec4 red(255, 0, 0, 255);
  vec4 green(0, 255, 0, 255);
  vec4 blue(0, 0, 255, 255);

  bool use_color = false;
  if(flag_ & kFlagColor) {
    use_color = true;
  }
  //x axis - r
  Append(vert_data, zero);
  if(use_color) {
    Append(vert_data, red);
  }
  Append(vert_data, xPos);
  if(use_color) {
    Append(vert_data, red);
  }

  //y axis - g
  Append(vert_data, zero);
  if(use_color) {
    Append(vert_data, green);
  }
  Append(vert_data, yPos);
  if(use_color) {
    Append(vert_data, green);
  }

  //z axis - b
  Append(vert_data, zero);
  if(use_color) {
    Append(vert_data, blue);
  }
  Append(vert_data, zPos);
  if(use_color) {
    Append(vert_data, blue);
  }
  return vert_data;
}
IndexListType PrimitiveModelBuilder::WireAxisIndexList() {
  IndexListType index_list;
  //create index list
  for(GLushort i = 0 ; i < 6 ; i++) {
    index_list.push_back(i);
  }
  return index_list;
}

std::vector<float> PrimitiveModelBuilder::WirePlaneVertexData() {
  float half_size = plane.half_size;
  float grid_size = plane.grid_size;
  vector<float> vert_list;

  int grid_range = (int)(half_size / grid_size);
  vert_list.resize((grid_range * 2 + 1) * 4 * 3);

  //left line(-half_size, 0, half_size) ~ (-half_size, 0, -half_size)
  //right line(+half_size, 0, half_size) ~ (+half_size, 0, -half_size)
  //front line(-half_size, 0, +half_size) ~ (+half_size, 0, +half_size)
  //back line(-half_size, 0, -half_size) ~ (+half_size, 0, -half_size)

  enum {
    kLeft = 0,
    kRight,
    kFront,
    kBack
  };

  //([0], 0, [1]) ~ ([2], 0, [3])
  float xz_axis_mark[][4] = {
    { -1, -1, -1, +1 },
    { +1, -1, +1, +1 },
    { -1, +1, +1, +1 },
    { -1, -1, +1, -1 },
  };
  for(int axis_idx = 0 ; axis_idx < 4 ; ++axis_idx) {
    int line_vert_size = grid_range * 2 + 1;
    for(int i = 0 ; i < line_vert_size ; ++i) {
      int start_index = line_vert_size * axis_idx;
      int vert_idx = start_index + i;
        
      float left_x = xz_axis_mark[axis_idx][0] * half_size;
      float right_x = xz_axis_mark[axis_idx][2] * half_size;
      float front_z = xz_axis_mark[axis_idx][3] * half_size;
      float back_z = xz_axis_mark[axis_idx][1] * half_size;
      SR_ASSERT(left_x <= right_x);
      SR_ASSERT(back_z <= front_z);

      float x, z;
      float scale = ((float)i / (line_vert_size-1.0f));
      if(left_x == right_x) {
        x = left_x;
      } else {
        x = scale * (right_x - left_x) + left_x;
      }
      if(front_z == back_z) {
        z = front_z;
      } else {
        z = scale * (front_z - back_z) + back_z;
      }

      vert_list[vert_idx*3 + 0] = x;
      vert_list[vert_idx*3 + 1] = 0;
      vert_list[vert_idx*3 + 2] = z;
    }
  }
  return vert_list;
}
IndexListType PrimitiveModelBuilder::WirePlaneIndexList() {
  float half_size = plane.half_size;
  float grid_size = plane.grid_size;
  IndexListType index_list;
  int grid_range = (int)(half_size / grid_size);
  enum {
    kLeft = 0,
    kRight,
    kFront,
    kBack
  };

  //build index list
  int line_vert_size = grid_range * 2 + 1;
  index_list.reserve(line_vert_size * 2 * 4);
  //left .... right를 잇는 선
  int left_start_index = line_vert_size * kLeft;
  int right_start_index = line_vert_size * kRight;
  for(int i = 0 ; i < line_vert_size ; i++) {
    index_list.push_back(left_start_index + i);
    index_list.push_back(right_start_index + i);
  }

  //front...back를 잇는 선
  int front_start_index = line_vert_size * kFront;
  int back_start_index = line_vert_size * kBack;
  for(int i = 0 ; i < line_vert_size ; i++) {
    index_list.push_back(front_start_index + i);
    index_list.push_back(back_start_index + i);
  }
  return index_list;
}

std::vector<float> PrimitiveModelBuilder::TeapotVertexData() {
  CalcTeapotSize();
  float size = teapot.size;
  vector<float> vert_data;

  //get bounding box to resize
  float orig_width = teapot_max_x_ - teapot_min_x_;
  float orig_height = teapot_max_y_ - teapot_min_y_;
  float orig_depth = teapot_max_z_ - teapot_min_z_;
  float max_size = orig_width > orig_height ? orig_width : orig_height;
  if(max_size < orig_depth) {
    max_size = orig_depth;
  }
  float scale = size / max_size;

  int pos_offset = 0;
  int color_offset = -1;
  int normal_offset = -1;
  int texcoord_offset = -1;
  int tangent_offset = -1;
  int rowsize = CalcOffset(flag_, &pos_offset, &color_offset, &normal_offset, &texcoord_offset, &tangent_offset);

  vert_data.resize(NUM_TEAPOT_OBJECT_VERTEX * rowsize);
  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_VERTEX ; i++) {
    vert_data[i*3+pos_offset+0] = teapotVertices[i * 3 + 0] * scale;
    vert_data[i*3+pos_offset+1] = teapotVertices[i * 3 + 1] * scale;
    vert_data[i*3+pos_offset+2] = teapotVertices[i * 3 + 2] * scale;

    if(color_offset != -1) {
      for(int j = 0 ; j < 4 ; ++j) {
        vert_data[i*3+color_offset+j] = 255;
      }
    }
    if(normal_offset != -1) {
      vert_data[i*3+normal_offset+0] = teapotNormals[i * 3 + 0];
      vert_data[i*3+normal_offset+1] = teapotNormals[i * 3 + 1];
      vert_data[i*3+normal_offset+2] = teapotNormals[i * 3 + 2];
    }
    if(texcoord_offset != -1) {
      vert_data[i*3+texcoord_offset+0] = teapotTexCoords[i * 3 + 0];
      vert_data[i*3+texcoord_offset+1] = teapotTexCoords[i * 3 + 1];
    }
  }
  return vert_data;
}
IndexListType PrimitiveModelBuilder::WireTeapotIndexList() {
  IndexListType index_list;
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
  return index_list;
}

IndexListType PrimitiveModelBuilder::SolidTeapotIndexList() {
  IndexListType index_list;
  index_list.reserve(NUM_TEAPOT_OBJECT_INDEX);
  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_INDEX ; i++) {
    int idx1 = teapotIndices[i];
    index_list.push_back(idx1);
  }
  return index_list;
}

std::vector<float> PrimitiveModelBuilder::WireConeVertexData() {
  float base = cone.base;
  int stacks = cone.stacks;
  float height = cone.height;
  int slices = cone.slices;

  //밑면/옆면을 다른 메시로 처리하자
  //vertex List는 공유하고 index만 다르게 하자
  vector<float> vert_data;

  //밑면
  vector<vec3> posList; 
  for(int j = 0 ; j < stacks ; j++) {
    float y = (1.0f / (float)stacks) * j;
    for(int i = 0 ; i < slices ; i++) {
      double angleDt = 2.0 * kPi / slices;
      float radius = (1.0f / stacks) * (stacks-j);
      float x = static_cast<float>(cos(angleDt * i) * radius);
      float z = static_cast<float>(sin(angleDt * i) * radius);

      vec3 pos(x, y, z);
      posList.push_back(pos);
    }
  }
  //최상위 꼭지점은 따로 넣는다
  vec3 toppos(0, 1, 0);
  posList.push_back(toppos);


  //vertex 구성
  //vec2 texCoord(0, 0);
  //vec3 normal(0, 0, 0);
  for(std::size_t i = 0 ; i < posList.size() ; i++) {
    vec3 &pos = posList[i];
    pos.x *= base;
    pos.y *= height;
    pos.z *= base;
    
    Append(vert_data, pos);
  }
  return vert_data;
}

IndexListType PrimitiveModelBuilder::WireConeIndexList() {
  float base = cone.base;
  int stacks = cone.stacks;
  float height = cone.height;
  int slices = cone.slices;

  IndexListType stackindex_list;
  IndexListType sliceindex_list;
  
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
  GLushort topVertexIndex = slices * stacks;
  for(int i = 0 ; i < slices ; i++) {
    sliceindex_list.push_back(i);
    sliceindex_list.push_back(topVertexIndex);
  }

  IndexListType index_list(stackindex_list.size() + sliceindex_list.size());
  auto it = copy(stackindex_list.begin(), stackindex_list.end(), index_list.begin());
  copy(sliceindex_list.begin(), sliceindex_list.end(), it);
  return index_list;
}

std::vector<float> PrimitiveModelBuilder::WireCylinderVertexData() {
  float radius = cylinder.radius;
  float height = cylinder.height;
  int slices = cylinder.slices;
  
  vector<float> vert_data;
  

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

      vec3 pos(x, y, z);
      Append(vert_data, pos);
    }
  }
  //밑점, 윗점 따로 vertex list에 넣기
  vec3 bottomPos(0, -height/2, 0);
  vec3 topPos(0, height/2, 0);
  Append(vert_data, bottomPos);
  Append(vert_data, topPos);

  return vert_data;
}
IndexListType PrimitiveModelBuilder::WireCylinderIndexList() {
  IndexListType index_list;
  float radius = cylinder.radius;
  float height = cylinder.height;
  int slices = cylinder.slices;

  int total_vert_count = 2 * slices + 2;

  const int topIndex = total_vert_count - 1;
  const int bottomIndex = total_vert_count - 2;

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
  return index_list;
}

std::vector<float> PrimitiveModelBuilder::SolidSphereVertexData() {
  float radius = sphere.radius;
  int slices = sphere.slices;
  int stacks = sphere.stacks;
  
  vector<float> vert_data;

  //원을 구성하는 포인트 리스트 + 텍스쳐 좌표 동시 계산
  vector<vec3> posList;
  vector<vec2> texCoordList;
  for(int i = 0 ; i <= stacks ; i++) {
    double yAngle = (kPi / stacks * i) - kPiOver2;
    float y = static_cast<float>(sin(yAngle));
    for(int j = 0 ; j <= slices ; j++) {
      float zxAngle = static_cast<float>((2.0 * kPi / slices) * j);
      float x = static_cast<float>(cos(yAngle) * cos(zxAngle));
      float z = static_cast<float>(cos(yAngle) * sin(zxAngle));

      vec3 pos(x, y, z);
      posList.push_back(pos);

      //tex 좌표 계산
      float s = 1.0f / (slices) * j;
      float t = 1.0f / (stacks) * i;
      vec2 texCoord(s, t);
      texCoordList.push_back(texCoord);
    }
  }

  int pos_offset = 0;
  int color_offset = -1;
  int normal_offset = -1;
  int texcoord_offset = -1;
  int tangent_offset = -1;
  int rowsize = CalcOffset(flag_, &pos_offset, &color_offset, &normal_offset, &texcoord_offset, &tangent_offset);

  //vertex 구성
  vert_data.resize(rowsize * posList.size());
  for(std::size_t i = 0 ; i < posList.size() ; i++) {
    const vec3 &pos = posList[i];
    const vec2 &texcoord = texCoordList[i];

    int base_idx = i * rowsize;
    if(pos_offset != -1) {
      vert_data[base_idx + pos_offset + 0] = pos.x * radius;
      vert_data[base_idx + pos_offset + 1] = pos.y * radius;
      vert_data[base_idx + pos_offset + 2] = pos.z * radius;
    }
    
    if(color_offset != -1) {
      for(int j = 0 ; j < 4 ; ++j) {
        vert_data[base_idx + color_offset + j] = 255;
      }
    }
    if(texcoord_offset != -1) {
      vert_data[base_idx + texcoord_offset + 0] = texcoord.x;
      vert_data[base_idx + texcoord_offset + 1] = texcoord.y;
    }
    if(normal_offset != -1) {
      vert_data[base_idx + normal_offset + 0] = pos.x;
      vert_data[base_idx + normal_offset + 1] = pos.y;
      vert_data[base_idx + normal_offset + 2] = pos.z;
    }
  }

  return vert_data;
}
IndexListType PrimitiveModelBuilder::SolidSphereIndexList() {
  float radius = sphere.radius;
  int slices = sphere.slices;
  int stacks = sphere.stacks;

  IndexListType index_list;
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
  return index_list;
}
}
