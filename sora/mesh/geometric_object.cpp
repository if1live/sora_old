/*
* Copyright (c) 2002-2008 LWJGL Project
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of 'LWJGL' nor the names of
*   its contributors may be used to endorse or promote products derived
*   from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "sora_stdafx.h"
#include "geometric_object.h"
#include "teapot.h"
#include "core/math_helper.h"

using namespace glm;
using namespace std;

namespace sora {;
void GeometricObject::PointCube(float width, float height, float depth) {
  SR_ASSERT(width > 0 && height > 0 && depth > 0);
  width = width/2;
  height = height/2;
  depth = depth/2;

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

  DrawCmdData cmd;
  cmd.draw_mode = kDrawPoints;
  for(size_t i = 0 ; i < pos_list.size() ; ++i) {
    Vertex vert;
    vert.pos = pos_list[i];
    cmd.vertex_list.push_back(vert);
  }
  this->cmd_list_.push_back(cmd);
}
void GeometricObject::WireCube(float width, float height, float depth) {
  SR_ASSERT(width > 0 && height > 0 && depth > 0);
  width = width/2;
  height = height/2;
  depth = depth/2;

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

  DrawCmdData cmd;
  cmd.draw_mode = kDrawLines;
  for(size_t i = 0 ; i < pos_list.size() ; ++i) {
    Vertex vert;
    vert.pos = pos_list[i];
    cmd.vertex_list.push_back(vert);
  }

  //GL_LINES용 index list
  std::array<GLushort, 24> index_list = {
    0,1,	1,3,	2,3,	0,2,
    4,5,	5,7,	6,7,	4,6,
    0,4,	1,5,	2,6,	3,7
  };
  cmd.index_list.resize(index_list.size());
  copy(index_list.begin(), index_list.end(), cmd.index_list.begin());
  this->cmd_list_.push_back(cmd);
}

void GeometricObject::SolidCube(float width, float height, float depth) {
  SR_ASSERT(width > 0 && height > 0 && depth > 0);
  width = width/2;
  height = height/2;
  depth = depth/2;

  DrawCmdData cmd;
  cmd.draw_mode = kDrawTriangles;
  VertexList &vert_list = cmd.vertex_list;
  IndexList &index_list = cmd.index_list;

  //normal
  {
    // Front Face
    int baseIndex = vert_list.size();
    vec3 normal(0, 0, +1);

    vec2 texCoord1(0, 0);	vec3 vertex1(-width, -height, depth);
    vec2 texCoord2(1, 0);	vec3 vertex2( width, -height, depth);
    vec2 texCoord3(1, 1);	vec3 vertex3( width,  height, depth);
    vec2 texCoord4(0, 1);	vec3 vertex4(-width,  height, depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;  v1.normal = normal;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;  v2.normal = normal;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;  v3.normal = normal;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;  v4.normal = normal;

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
    vec3 normal(0, 0, -1);

    vec2 texCoord1(1, 0);	vec3 vertex1(-width, -height, -depth);
    vec2 texCoord2(1, 1);	vec3 vertex2(-width,  height, -depth);
    vec2 texCoord3(0, 1);	vec3 vertex3( width,  height, -depth);
    vec2 texCoord4(0, 0);	vec3 vertex4( width, -height, -depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;  v1.normal = normal;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;  v2.normal = normal;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;  v3.normal = normal;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;  v4.normal = normal;

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
    vec3 normal(0, 1, 0);

    vec2 texCoord1(0, 1);	vec3 vertex1(-width, height, -depth);
    vec2 texCoord2(0, 0);	vec3 vertex2(-width, height,  depth);
    vec2 texCoord3(1, 0);	vec3 vertex3( width, height,  depth);
    vec2 texCoord4(1, 1);	vec3 vertex4( width, height, -depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;  v1.normal = normal;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;  v2.normal = normal;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;  v3.normal = normal;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;  v4.normal = normal;

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
    vec3 normal(0, -1, 0);

    vec2 texCoord1(1, 1);	vec3 vertex1(-width, -height, -depth);
    vec2 texCoord2(0, 1);	vec3 vertex2( width, -height, -depth);
    vec2 texCoord3(0, 0);	vec3 vertex3( width, -height,  depth);
    vec2 texCoord4(1, 0);	vec3 vertex4(-width, -height,  depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;  v1.normal = normal;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;  v2.normal = normal;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;  v3.normal = normal;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;  v4.normal = normal;

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
    vec3 normal(1, 0, 0);

    vec2 texCoord1(1, 0);	vec3 vertex1(width, -height, -depth);
    vec2 texCoord2(1, 1);	vec3 vertex2(width,  height, -depth);
    vec2 texCoord3(0, 1);	vec3 vertex3(width,  height,  depth);
    vec2 texCoord4(0, 0);	vec3 vertex4(width, -height,  depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;  v1.normal = normal;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;  v2.normal = normal;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;  v3.normal = normal;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;  v4.normal = normal;

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
    vec3 normal(0, -1, 0);

    vec2 texCoord1(0, 0);	vec3 vertex1(-width, -height, -depth);
    vec2 texCoord2(1, 0);	vec3 vertex2(-width, -height,  depth);
    vec2 texCoord3(1, 1);	vec3 vertex3(-width,  height,  depth);
    vec2 texCoord4(0, 1);	vec3 vertex4(-width,  height, -depth);

    //add vertex
    Vertex v1;  v1.pos = vertex1; v1.texcoord = texCoord1;  v1.normal = normal;
    Vertex v2;  v2.pos = vertex2; v2.texcoord = texCoord2;  v2.normal = normal;
    Vertex v3;  v3.pos = vertex3; v3.texcoord = texCoord3;  v3.normal = normal;
    Vertex v4;  v4.pos = vertex4; v4.texcoord = texCoord4;  v4.normal = normal;

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
  this->cmd_list_.push_back(cmd);
}

void GeometricObject::PointTeapot( float size ) {
  DrawCmdData cmd;
  cmd.draw_mode = kDrawPoints;
  cmd.vertex_list.resize(NUM_TEAPOT_OBJECT_VERTEX);
  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_VERTEX ; i++) {
    Vertex &vert = cmd.vertex_list[i];
    vert.pos.x = teapotVertices[i*3+0] * size;
    vert.pos.y = teapotVertices[i*3+1] * size;
    vert.pos.z = teapotVertices[i*3+2] * size;

    vert.texcoord.x = teapotTexCoords[i*2+0];
    vert.texcoord.y = teapotTexCoords[i*2+1];

    vert.normal.x = teapotNormals[i*3+0];
    vert.normal.y = teapotNormals[i*3+1];
    vert.normal.z = teapotNormals[i*3+2];
  }
  this->cmd_list_.push_back(cmd);
}

void GeometricObject::WireTeapot( float size ) {
  DrawCmdData cmd;
  cmd.draw_mode = kDrawLines;
  cmd.vertex_list.resize(NUM_TEAPOT_OBJECT_VERTEX);
  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_VERTEX ; i++) {
    Vertex &vert = cmd.vertex_list[i];
    vert.pos.x = teapotVertices[i*3+0] * size;
    vert.pos.y = teapotVertices[i*3+1] * size;
    vert.pos.z = teapotVertices[i*3+2] * size;

    vert.texcoord.x = teapotTexCoords[i*2+0];
    vert.texcoord.y = teapotTexCoords[i*2+1];

    vert.normal.x = teapotNormals[i*3+0];
    vert.normal.y = teapotNormals[i*3+1];
    vert.normal.z = teapotNormals[i*3+2];
  }

  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_INDEX / 3 ; i++) {
    unsigned short idx_1 = teapotIndices[i*3+0];
    unsigned short idx_2 = teapotIndices[i*3+1];
    unsigned short idx_3 = teapotIndices[i*3+2];
    cmd.index_list.push_back(idx_1);
    cmd.index_list.push_back(idx_2);
    cmd.index_list.push_back(idx_2);
    cmd.index_list.push_back(idx_3);
    cmd.index_list.push_back(idx_3);
    cmd.index_list.push_back(idx_1);
  }
  this->cmd_list_.push_back(cmd);
}

void GeometricObject::SolidTeapot( float size ) {
  DrawCmdData cmd;
  cmd.draw_mode = kDrawTriangles;
  cmd.vertex_list.resize(NUM_TEAPOT_OBJECT_VERTEX);
  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_VERTEX ; i++) {
    Vertex &vert = cmd.vertex_list[i];
    vert.pos.x = teapotVertices[i*3+0] * size;
    vert.pos.y = teapotVertices[i*3+1] * size;
    vert.pos.z = teapotVertices[i*3+2] * size;

    vert.texcoord.x = teapotTexCoords[i*2+0];
    vert.texcoord.y = teapotTexCoords[i*2+1];

    vert.normal.x = teapotNormals[i*3+0];
    vert.normal.y = teapotNormals[i*3+1];
    vert.normal.z = teapotNormals[i*3+2];
  }

  cmd.index_list.resize(NUM_TEAPOT_OBJECT_INDEX);
  memcpy(&cmd.index_list[0], teapotIndices, sizeof(teapotIndices));
  this->cmd_list_.push_back(cmd);
}

//http://massapi.com/source/lwjgl-source-2.7.1/src/java/org/lwjgl/util/glu/Sphere.java.html
void GeometricObject::PointShpere(float radius, int slices, int stacks) {
  float nsign = 1.0f;
  float drho = kPi / stacks;
  float dtheta = 2.0f * kPi / slices;

  DrawCmdData cmd;
  cmd.draw_mode = kDrawPoints;

  // top and bottom-most points
  Vertex vert_top;
  vert_top.pos = vec3(0.0f, 0.0f, radius);
  vert_top.normal = vec3(0.0f, 0.0f, nsign);;
  cmd.vertex_list.push_back(vert_top);

  Vertex vert_bottom;
  vert_bottom.normal = vec3(0.0f, 0.0f, -nsign);
  vert_bottom.pos = vec3(0.0f, 0.0f, -radius);
  cmd.vertex_list.push_back(vert_bottom);

  // loop over stacks
  for (int i = 1; i < stacks - 1; i++) {
    float rho = i * drho;
    for (int j = 0; j < slices; j++) {
      float theta = j * dtheta;
      float x = cos(theta) * sin(rho);
      float y = sin(theta) * sin(rho);
      float z = cos(rho);

      Vertex vert;
      vert.normal = vec3(x * nsign, y * nsign, z * nsign);
      vert.pos = vec3(x * radius, y * radius, z * radius);
      cmd.vertex_list.push_back(vert);
    }
  }
  this->cmd_list_.push_back(cmd);
}
void GeometricObject::WireShpere(float radius, int slices, int stacks) {
  float nsign = 1.0f;
  float drho = sora::kPi / stacks;
  float dtheta = 2.0f * sora::kPi / slices;

  // draw stack lines
  for (int i = 1 ; i < stacks ; i++) { // stack line at i==stacks-1 was missing here
    float rho = i * drho;

    DrawCmdData cmd;
    cmd.draw_mode = kDrawLineLoop;
    for (int j = 0; j < slices; j++) {
      float theta = j * dtheta;
      float x = cos(theta) * sin(rho);
      float y = sin(theta) * sin(rho);
      float z = cos(rho);

      Vertex vert;
      vert.normal.x = x * nsign;
      vert.normal.y = y * nsign;
      vert.normal.z = z * nsign;

      vert.pos.x = x * radius;
      vert.pos.y = y * radius;
      vert.pos.z = z * radius;
      cmd.vertex_list.push_back(vert);
    }
    this->cmd_list_.push_back(cmd);
  }
  // draw slice lines
  for (int j = 0; j < slices; j++) {
    float theta = j * dtheta;

    DrawCmdData cmd;
    cmd.draw_mode = kDrawLineStrip;
    for (int i = 0; i <= stacks; i++) {
      float rho = i * drho;
      float x = cos(theta) * sin(rho);
      float y = sin(theta) * sin(rho);
      float z = cos(rho);

      Vertex vert;
      vert.normal.x = x * nsign;
      vert.normal.y = y * nsign;
      vert.normal.z = z * nsign;

      vert.pos.x = x * radius;
      vert.pos.y = y * radius;
      vert.pos.z = z * radius;
      cmd.vertex_list.push_back(vert);
    }
    this->cmd_list_.push_back(cmd);
  }
}

void GeometricObject::SolidSphere(float radius, int slices, int stacks) {
  bool normals = true;
  float nsign = 1.0f;
  float drho = kPi / stacks;
  float dtheta = 2.0f * kPi / slices;

  float ds = 1.0f / slices;
  float dt = 1.0f / stacks;
  float t = 1.0f; // because loop now runs from 0
  int imin = 0;
  int imax = stacks;

  // draw intermediate stacks as quad strips
  for (int i = imin; i < imax; i++) {
    float rho = i * drho;

    //quad strip로 구성된 vertex 목록 구성하기
    VertexList vert_list;
    float s = 0.0f;
    for (int j = 0; j <= slices; j++) {
      float theta = (j == slices) ? 0.0f : j * dtheta;
      float x = -sin(theta) * sin(rho);
      float y = cos(theta) * sin(rho);
      float z = nsign * cos(rho);

      Vertex vert1;
      vert1.normal = vec3(x * nsign, y * nsign, z * nsign);
      vert1.texcoord = vec2(s, t);
      vert1.pos = vec3(x * radius, y * radius, z * radius);
      vert_list.push_back(vert1);

      x = -sin(theta) * sin(rho + drho);
      y = cos(theta) * sin(rho + drho);
      z = nsign * cos(rho + drho);

      Vertex vert2;
      vert2.normal = vec3(x * nsign, y * nsign, z * nsign);
      vert2.texcoord = vec2(s, t - dt);
      s += ds;
      vert2.pos = vec3(x * radius, y * radius, z * radius);
      vert_list.push_back(vert2);
    }

    DrawCmdData cmd;
    cmd.draw_mode = kDrawTriangleStrip;
    //quad strip -> triangle strip
    cmd.vertex_list = vert_list;
    this->cmd_list_.push_back(cmd);

    t -= dt;
  }
}


//http://massapi.com/source/lwjgl-source-2.7.1/src/java/org/lwjgl/util/glu/Cylinder.java.html
void GeometricObject::PointCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
  float nsign = 1.0f;
  float da = 2.0f * kPi / slices;
  float dr = (topRadius - baseRadius) / stacks;
  float dz = height / stacks;
  float nz = (baseRadius - topRadius) / height;
  // Z component of normal vectors

  DrawCmdData cmd;
  cmd.draw_mode = kDrawPoints;

  for (int i = 0; i < slices; i++) {
    float x = cos((i * da));
    float y = sin((i * da));

    Vertex vert;
    vert.normal = vec3(x * nsign, y * nsign, nz * nsign);

    float z = 0.0f;
    float r = baseRadius;
    for (int j = 0; j <= stacks; j++) {
      vert.pos = vec3((x * r), (y * r), z);
      cmd.vertex_list.push_back(vert);

      z += dz;
      r += dr;
    }
  }
  
  this->cmd_list_.push_back(cmd);
}
void GeometricObject::WireCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
  float nsign = 1.0f;
  float da = 2.0f * kPi / slices;
  float dr = (topRadius - baseRadius) / stacks;
  float dz = height / stacks;
  float nz = (baseRadius - topRadius) / height;
  // Z component of normal vectors

  // Draw rings
  float z = 0.0f;
  float r = baseRadius;
  for (int j = 0; j <= stacks; j++) {
    DrawCmdData cmd;
    cmd.draw_mode = kDrawLineLoop;
    for (int i = 0; i < slices; i++) {
      float x = cos((i * da));
      float y = sin((i * da));
        
      Vertex vert;
      vert.normal = vec3(x * nsign, y * nsign, nz * nsign);
      vert.pos = vec3((x * r), (y * r), z); 
      cmd.vertex_list.push_back(vert);
    }
    this->cmd_list_.push_back(cmd);
    z += dz;
    r += dr;
  }
  {
    // draw length lines
    DrawCmdData cmd;
    cmd.draw_mode = kDrawLines;
    for (int i = 0; i < slices; i++) {
      float x = cos((i * da));
      float y = sin((i * da));

      Vertex vert;
      vert.normal = vec3(x * nsign, y * nsign, nz * nsign);
      vert.pos = vec3((x * baseRadius), (y * baseRadius), 0.0f);
      cmd.vertex_list.push_back(vert);

      vert.pos = vec3((x * topRadius), (y * topRadius), (height));
      cmd.vertex_list.push_back(vert);
    }
    this->cmd_list_.push_back(cmd);
  }
}
void GeometricObject::SolidCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
  float nsign = 1.0f;
  float da = 2.0f * kPi / slices;
  float dr = (topRadius - baseRadius) / stacks;
  float dz = height / stacks;
  float nz = (baseRadius - topRadius) / height;
  // Z component of normal vectors

  float ds = 1.0f / slices;
  float dt = 1.0f / stacks;
  float t = 0.0f;
  float z = 0.0f;
  float r = baseRadius;
  for (int j = 0; j < stacks; j++) {
    float s = 0.0f;

    DrawCmdData cmd;
    cmd.draw_mode = kDrawTriangleStrip;  //사실은 GL_QUAD_STRIP
    for (int i = 0; i <= slices; i++) {
      float x, y;
      if (i == slices) {
        x = sin(0.0f);
        y = cos(0.0f);
      } else {
        x = sin((i * da));
        y = cos((i * da));
      }
      if (nsign == 1.0f) {
        Vertex vert;
        vert.normal = vec3((x * nsign), (y * nsign), (nz * nsign));
        vert.texcoord = vec2(s, t);
        vert.pos = vec3((x * r), (y * r), z);
        cmd.vertex_list.push_back(vert);

        vert.normal = vec3((x * nsign), (y * nsign), (nz * nsign));
        vert.texcoord = vec2(s, t + dt);
        vert.pos = vec3((x * (r + dr)), (y * (r + dr)), (z + dz));
        cmd.vertex_list.push_back(vert);
      } else {
        Vertex vert;
        vert.normal = vec3(x * nsign, y * nsign, nz * nsign);
        vert.texcoord = vec2(s, t);
        vert.pos = vec3((x * r), (y * r), z);
        cmd.vertex_list.push_back(vert);

        vert.normal = vec3(x * nsign, y * nsign, nz * nsign);
        vert.texcoord = vec2(s, t + dt);
        vert.pos = vec3((x * (r + dr)), (y * (r + dr)), (z + dz));
        cmd.vertex_list.push_back(vert);
      }
      s += ds;
    } // for slices
    this->cmd_list_.push_back(cmd);
    r += dr;
    t += dt;
    z += dz;
  } // for stacks
}

void GeometricObject::WireAxis(float size) {
  DrawCmdData cmd;
  cmd.draw_mode = kDrawLines;
  VertexList &vert_list = cmd.vertex_list;

  //vertex list 생성
  vec3 xPos(size, 0, 0);
  vec3 yPos(0, size, 0);
  vec3 zPos(0, 0, size);
  vec3 zero(0, 0, 0);

  sora::vec4ub red(255, 0, 0, 255);
  sora::vec4ub green(0, 255, 0, 255);
  sora::vec4ub blue(0, 0, 255, 255);
  
  {
    // x axis - r
    Vertex x_zero_vert;
    x_zero_vert.color = red;
    x_zero_vert.pos = zero;
    vert_list.push_back(x_zero_vert);

    Vertex x_one_vert;
    x_one_vert.color = red;
    x_one_vert.pos = xPos;
    vert_list.push_back(x_one_vert);
  }
  {
    //y axis - g
    Vertex zero_vert;
    zero_vert.color = green;
    zero_vert.pos = zero;
    vert_list.push_back(zero_vert);

    Vertex y_vert;
    y_vert.color = green;
    y_vert.pos = yPos;
    vert_list.push_back(y_vert);
  }
  {
    //z axis - b
    Vertex zero_vert;
    zero_vert.color = blue;
    zero_vert.pos = zero;
    vert_list.push_back(zero_vert);

    Vertex z_vert;
    z_vert.color = blue;
    z_vert.pos = zPos;
    vert_list.push_back(z_vert);
  }
  this->cmd_list_.push_back(cmd);
}

void GeometricObject::WirePlane(float half_size, float grid_size) {
  DrawCmdData cmd;
  cmd.draw_mode = kDrawLines;
  VertexList &vert_list = cmd.vertex_list;

  int grid_range = (int)(half_size / grid_size);

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

      Vertex vert;
      vert.pos = vec3(x, 0, z);
      vert_list.push_back(vert);
    }
  }

  //////////////////////////
  IndexList &index_list = cmd.index_list;

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
  this->cmd_list_.push_back(cmd);
}

void GeometricObject::SolidPlane(float half_size) {

  DrawCmdData cmd;
  cmd.draw_mode = kDrawTriangles;
  VertexList &vert_list = cmd.vertex_list;

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

    vec3 pos(x_scale * half_size, 0, z_scale * half_size);
    vec2 texcoord(s, t);
    vec3 normal(0, 1, 0);

    Vertex vert;
    vert.normal = normal;
    vert.pos = pos;
    vert.texcoord = texcoord;
    cmd.vertex_list.push_back(vert);
  }

  ///////////////////////
  enum {
    kLeftBack,
    kLeftFront,
    kRightBack,
    kRightFront,
  };

  IndexList &index_list = cmd.index_list;
  index_list.reserve(6);
  index_list.push_back(kLeftBack); 
  index_list.push_back(kLeftFront);  
  index_list.push_back(kRightBack);  

  index_list.push_back(kRightBack);
  index_list.push_back(kLeftFront);
  index_list.push_back(kRightFront);

  ///////////
  this->cmd_list_.push_back(cmd);
}

} //namespace sora