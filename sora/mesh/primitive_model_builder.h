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
#ifndef SORA_PRIMITIVE_MODEL_BUILDER_H_
#define SORA_PRIMITIVE_MODEL_BUILDER_H_

#include "mesh/vertex.h"
#include "renderer/mesh.h"
#include <vector>

namespace sora {;
typedef std::vector<ushort> IndexListType;

//1개의 vertex list, 여러개의 index list로 primitive목록을 구성한다
//반환형은 float list로 고정해서 필요한 vertex 타입에 따라서 후처리해서 쓸수 있도록하자
class PrimitiveModelBuilder {
public:
  enum {
    kFlagColor = 1 << 0,
    kFlagNormal = 1 << 1,
    kFlagTexcoord = 1 << 2,
    kFlagTangent = 1 << 3
  };
  PrimitiveModelBuilder(unsigned int flag);

  void SetCube(float width, float height, float depth);
  void SetSphere(float radius, int slices, int stacks);
  void SetAxis(float size);
  void SetPlane(float half_size, float grid_size);
  void SetTeapot(float size);
  void SetCone(float base, float height, int slices, int stacks);
  void SetCylinder(float radius, float height, int slices);

  //공통부분 복사
  template<typename VertexType>
  static void DataToCommonVertexList(const std::vector<float> &data, uint flag, std::vector<VertexType> &vert_list);
  static void DataToVertexList(const std::vector<float> &data, uint flag, std::vector<Vertex> &vert_list);
  static void DataToVertexList(const std::vector<float> &data, uint flag, std::vector<TangentVertex> &vert_list);
  static int CalcOffset(uint flag, int *pos, int *color, int *normal, int *texcoord, int *tangent);
  

  std::vector<float> WireCubeVertexData();
  IndexListType WireCubeIndexList();

  std::vector<float> WireSphereVertexData();
  IndexListType WireSphereIndexList();

  std::vector<float> SolidSphereVertexData();
  IndexListType SolidSphereIndexList();

  std::vector<float> WireAxisVertexData();
  IndexListType WireAxisIndexList();

  std::vector<float> WirePlaneVertexData();
  IndexListType WirePlaneIndexList();
  std::vector<float> SolidPlaneVertexData();
  IndexListType SolidPlaneIndexList();

  std::vector<float> TeapotVertexData();
  IndexListType WireTeapotIndexList();
  IndexListType SolidTeapotIndexList();

  std::vector<float> WireConeVertexData();
  IndexListType WireConeIndexList();

  std::vector<float> WireCylinderVertexData();
  IndexListType WireCylinderIndexList();

  uint flag() const { return flag_; }

public:
  template<typename VertexType>
  static MeshBufferObject CreateWireCube(uint flag, float width, float height, float depth);


private:
  unsigned int flag_;
  union {
    struct {
      float width, height, depth;
    } cube;
    struct {
      float radius;
      int slices;
      int stacks;
    } sphere;
    struct {
      float size;
    } axis;
    struct {
      float half_size;
      float grid_size;
    } plane;
    struct {
      float size;
    } teapot;
    struct {
      float base;
      float height;
      int slices;
      int stacks;
    } cone;
    struct {
      float radius;
      float height;
      int slices;
    } cylinder;
  };

  void Append(std::vector<float> &vert_data, const glm::vec2 &v);
  void Append(std::vector<float> &vert_data, const glm::vec3 &v);
  void Append(std::vector<float> &vert_data, const glm::vec4 &v);

private:
  static void CalcTeapotSize();
  //teapot
  static float teapot_min_x_;
  static float teapot_max_x_;
  static float teapot_min_y_;
  static float teapot_max_y_;
  static float teapot_min_z_;
  static float teapot_max_z_;
  static bool teapot_created_;
};

template<typename VertexType>
void PrimitiveModelBuilder::DataToCommonVertexList(const std::vector<float> &data, uint flag, std::vector<VertexType> &vert_list) {
  int pos_offset = 0;
  int color_offset = -1;
  int normal_offset = -1;
  int texcoord_offset = -1;
  int tangent_offset = -1;
  int rowsize = CalcOffset(flag, &pos_offset, &color_offset, &normal_offset, &texcoord_offset, &tangent_offset);
  
  for(size_t i = 0 ; i < data.size() / rowsize ; ++i) {
    VertexType vert;
    int pos_idx = i * rowsize + pos_offset;
    vert.pos.x = data[pos_idx + 0];
    vert.pos.y = data[pos_idx + 1];
    vert.pos.z = data[pos_idx + 2];
    
    if(color_offset != -1) {
      int color_idx = i * rowsize + color_offset;
      vert.color.x = data[color_idx + 0];
      vert.color.y = data[color_idx + 1];
      vert.color.z = data[color_idx + 2];
      vert.color.w = data[color_idx + 3];
    } else {
      vert.color = vec4ub(255);
    }

    if(normal_offset != -1) {
      int normal_idx = i * rowsize + normal_offset;
      vert.normal.x = data[normal_idx + 0];
      vert.normal.y = data[normal_idx + 1];
      vert.normal.z = data[normal_idx + 2];

    } else {
      vert.normal = vec3(0, 1, 0);
    }

    if(texcoord_offset != -1) {
      int texcoord_idx = i * rowsize + texcoord_offset;
      vert.texcoord.x = data[texcoord_idx + 0];
      vert.texcoord.y = data[texcoord_idx + 1];
    } else {
      vert.texcoord = vec2(0);
    }
    vert_list.push_back(vert);
  }
}

template<typename VertexType>
MeshBufferObject PrimitiveModelBuilder::CreateWireCube(uint flag, float width, float height, float depth) {
  PrimitiveModelBuilder builder(flag);
  builder.SetCube(width, height, depth);
  vector<float> vert_data = builder.WireCubeVertexData();
  IndexListType index_list = builder.WireCubeIndexList();

  vector<TangentVertex> vertex_list;
  builder.DataToVertexList(vert_data, builder.flag(), vertex_list);

  DrawCommand<VertexType> draw_cmd;
  draw_cmd.draw_mode = GL_LINES;
  draw_cmd.index_count = index_list.size();
  draw_cmd.index_ptr = &index_list[0];
  draw_cmd.index_type = GL_UNSIGNED_SHORT;
  draw_cmd.vert_count = vertex_list.size();
  draw_cmd.vert_ptr = &vertex_list[0];

  MeshBufferObject mbo;
  mbo.Add(draw_cmd);
  return mbo;
}
}

#endif  // SORA_PRIMITIVE_MODEL_BUILDER_H_