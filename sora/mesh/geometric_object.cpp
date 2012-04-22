/*
* freeglut_geometry.c
*
* Freeglut geometry rendering methods.
*
* Copyright (c) 1999-2000 Pawel W. Olszta. All Rights Reserved.
* Written by Pawel W. Olszta, <olszta@sourceforge.net>
* Creation date: Fri Dec 3 1999
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
* PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

  //GL_LINES侩 index list
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

    //quad strip肺 备己等 vertex 格废 备己窍扁
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
#if 0
/*
* Draws a wireframed cube. Code contributed by Andreas Umbach <marvin@dataway.ch>
*/

void glutWireCube( float dSize )
{
  double size = dSize * 0.5;

#   define V(a,b,c) glVertex3d( a size, b size, c size );
#   define N(a,b,c) glNormal3d( a, b, c );

  /* PWO: I dared to convert the code to use macros... */
  glBegin( GL_LINE_LOOP ); N( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+); glEnd();
  glBegin( GL_LINE_LOOP ); N( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+); glEnd();
  glBegin( GL_LINE_LOOP ); N( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+); glEnd();
  glBegin( GL_LINE_LOOP ); N(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-); glEnd();
  glBegin( GL_LINE_LOOP ); N( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+); glEnd();
  glBegin( GL_LINE_LOOP ); N( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-); glEnd();

#   undef V
#   undef N
}
#endif
#if 0
/*
* Draws a solid cube. Code contributed by Andreas Umbach <marvin@dataway.ch>
*/
void  glutSolidCube( float dSize )
{
  double size = dSize * 0.5;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidCube" );

#   define V(a,b,c) glVertex3d( a size, b size, c size );
#   define N(a,b,c) glNormal3d( a, b, c );

  /* PWO: Again, I dared to convert the code to use macros... */
  glBegin( GL_QUADS );
  N( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+);
  N( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+);
  N( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+);
  N(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-);
  N( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+);
  N( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-);
  glEnd();

#   undef V
#   undef N
}

/*
* Compute lookup table of cos and sin values forming a cirle
*
* Notes:
*    It is the responsibility of the caller to free these tables
*    The size of the table is (n+1) to form a connected loop
*    The last entry is exactly the same as the first
*    The sign of n can be flipped to get the reverse loop
*/

static void fghCircleTable(double **sint,double **cost,const int n)
{
  int i;

  /* Table size, the sign of n flips the circle direction */

  const int size = abs(n);

  /* Determine the angle between samples */

  const double angle = 2*M_PI/(double)( ( n == 0 ) ? 1 : n );

  /* Allocate memory for n samples, plus duplicate of first entry at the end */

  *sint = (double *) calloc(sizeof(double), size+1);
  *cost = (double *) calloc(sizeof(double), size+1);

  /* Bail out if memory allocation fails, fgError never returns */

  if (!(*sint) || !(*cost))
  {
    free(*sint);
    free(*cost);
    fgError("Failed to allocate memory in fghCircleTable");
  }

  /* Compute cos and sin around the circle */

  (*sint)[0] = 0.0;
  (*cost)[0] = 1.0;

  for (i=1; i<size; i++)
  {
    (*sint)[i] = sin(angle*i);
    (*cost)[i] = cos(angle*i);
  }

  /* Last sample is duplicate of the first */

  (*sint)[size] = (*sint)[0];
  (*cost)[size] = (*cost)[0];
}

/*
* Draws a solid sphere
*/
void  glutSolidSphere(float radius, GLint slices, GLint stacks)
{
  int i,j;

  /* Adjust z and radius as stacks are drawn. */

  double z0,z1;
  double r0,r1;

  /* Pre-computed circle */

  double *sint1,*cost1;
  double *sint2,*cost2;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidSphere" );

  fghCircleTable(&sint1,&cost1,-slices);
  fghCircleTable(&sint2,&cost2,stacks*2);

  /* The top stack is covered with a triangle fan */

  z0 = 1.0;
  z1 = cost2[(stacks>0)?1:0];
  r0 = 0.0;
  r1 = sint2[(stacks>0)?1:0];

  glBegin(GL_TRIANGLE_FAN);

  glNormal3d(0,0,1);
  glVertex3d(0,0,radius);

  for (j=slices; j>=0; j--)
  {
    glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
    glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
  }

  glEnd();

  /* Cover each stack with a quad strip, except the top and bottom stacks */

  for( i=1; i<stacks-1; i++ )
  {
    z0 = z1; z1 = cost2[i+1];
    r0 = r1; r1 = sint2[i+1];

    glBegin(GL_QUAD_STRIP);

    for(j=0; j<=slices; j++)
    {
      glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
      glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
      glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
      glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
    }

    glEnd();
  }

  /* The bottom stack is covered with a triangle fan */

  z0 = z1;
  r0 = r1;

  glBegin(GL_TRIANGLE_FAN);

  glNormal3d(0,0,-1);
  glVertex3d(0,0,-radius);

  for (j=0; j<=slices; j++)
  {
    glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
    glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
  }

  glEnd();

  /* Release sin and cos tables */

  free(sint1);
  free(cost1);
  free(sint2);
  free(cost2);
}

/*
* Draws a wire sphere
*/
void  glutWireSphere(float radius, GLint slices, GLint stacks)
{
  int i,j;

  /* Adjust z and radius as stacks and slices are drawn. */

  double r;
  double x,y,z;

  /* Pre-computed circle */

  double *sint1,*cost1;
  double *sint2,*cost2;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutWireSphere" );

  fghCircleTable(&sint1,&cost1,-slices  );
  fghCircleTable(&sint2,&cost2, stacks*2);

  /* Draw a line loop for each stack */

  for (i=1; i<stacks; i++)
  {
    z = cost2[i];
    r = sint2[i];

    glBegin(GL_LINE_LOOP);

    for(j=0; j<=slices; j++)
    {
      x = cost1[j];
      y = sint1[j];

      glNormal3d(x,y,z);
      glVertex3d(x*r*radius,y*r*radius,z*radius);
    }

    glEnd();
  }

  /* Draw a line loop for each slice */

  for (i=0; i<slices; i++)
  {
    glBegin(GL_LINE_STRIP);

    for(j=0; j<=stacks; j++)
    {
      x = cost1[i]*sint2[j];
      y = sint1[i]*sint2[j];
      z = cost2[j];

      glNormal3d(x,y,z);
      glVertex3d(x*radius,y*radius,z*radius);
    }

    glEnd();
  }

  /* Release sin and cos tables */

  free(sint1);
  free(cost1);
  free(sint2);
  free(cost2);
}

/*
* Draws a solid cone
*/
void  glutSolidCone( float base, float height, GLint slices, GLint stacks )
{
  int i,j;

  /* Step in z and radius as stacks are drawn. */

  double z0,z1;
  double r0,r1;

  const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );
  const double rStep = base / ( ( stacks > 0 ) ? stacks : 1 );

  /* Scaling factors for vertex normals */

  const double cosn = ( height / sqrt ( height * height + base * base ));
  const double sinn = ( base   / sqrt ( height * height + base * base ));

  /* Pre-computed circle */

  double *sint,*cost;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidCone" );

  fghCircleTable(&sint,&cost,-slices);

  /* Cover the circular base with a triangle fan... */

  z0 = 0.0;
  z1 = zStep;

  r0 = base;
  r1 = r0 - rStep;

  glBegin(GL_TRIANGLE_FAN);

  glNormal3d(0.0,0.0,-1.0);
  glVertex3d(0.0,0.0, z0 );

  for (j=0; j<=slices; j++)
    glVertex3d(cost[j]*r0, sint[j]*r0, z0);

  glEnd();

  /* Cover each stack with a quad strip, except the top stack */

  for( i=0; i<stacks-1; i++ )
  {
    glBegin(GL_QUAD_STRIP);

    for(j=0; j<=slices; j++)
    {
      glNormal3d(cost[j]*cosn, sint[j]*cosn, sinn);
      glVertex3d(cost[j]*r0,   sint[j]*r0,   z0  );
      glVertex3d(cost[j]*r1,   sint[j]*r1,   z1  );
    }

    z0 = z1; z1 += zStep;
    r0 = r1; r1 -= rStep;

    glEnd();
  }

  /* The top stack is covered with individual triangles */

  glBegin(GL_TRIANGLES);

  glNormal3d(cost[0]*sinn, sint[0]*sinn, cosn);

  for (j=0; j<slices; j++)
  {
    glVertex3d(cost[j+0]*r0,   sint[j+0]*r0,   z0    );
    glVertex3d(0,              0,              height);
    glNormal3d(cost[j+1]*sinn, sint[j+1]*sinn, cosn  );
    glVertex3d(cost[j+1]*r0,   sint[j+1]*r0,   z0    );
  }

  glEnd();

  /* Release sin and cos tables */

  free(sint);
  free(cost);
}

/*
* Draws a wire cone
*/
void  glutWireCone( float base, float height, GLint slices, GLint stacks)
{
  int i,j;

  /* Step in z and radius as stacks are drawn. */

  double z = 0.0;
  double r = base;

  const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );
  const double rStep = base / ( ( stacks > 0 ) ? stacks : 1 );

  /* Scaling factors for vertex normals */

  const double cosn = ( height / sqrt ( height * height + base * base ));
  const double sinn = ( base   / sqrt ( height * height + base * base ));

  /* Pre-computed circle */

  double *sint,*cost;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutWireCone" );

  fghCircleTable(&sint,&cost,-slices);

  /* Draw the stacks... */

  for (i=0; i<stacks; i++)
  {
    glBegin(GL_LINE_LOOP);

    for( j=0; j<slices; j++ )
    {
      glNormal3d(cost[j]*sinn, sint[j]*sinn, cosn);
      glVertex3d(cost[j]*r,    sint[j]*r,    z   );
    }

    glEnd();

    z += zStep;
    r -= rStep;
  }

  /* Draw the slices */

  r = base;

  glBegin(GL_LINES);

  for (j=0; j<slices; j++)
  {
    glNormal3d(cost[j]*sinn, sint[j]*sinn, cosn  );
    glVertex3d(cost[j]*r,    sint[j]*r,    0.0   );
    glVertex3d(0.0,          0.0,          height);
  }

  glEnd();

  /* Release sin and cos tables */

  free(sint);
  free(cost);
}


/*
* Draws a solid cylinder
*/
void  glutSolidCylinder(float radius, float height, GLint slices, GLint stacks)
{
  int i,j;

  /* Step in z and radius as stacks are drawn. */

  double z0,z1;
  const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );

  /* Pre-computed circle */

  double *sint,*cost;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidCylinder" );

  fghCircleTable(&sint,&cost,-slices);

  /* Cover the base and top */

  glBegin(GL_TRIANGLE_FAN);
  glNormal3d(0.0, 0.0, -1.0 );
  glVertex3d(0.0, 0.0,  0.0 );
  for (j=0; j<=slices; j++)
    glVertex3d(cost[j]*radius, sint[j]*radius, 0.0);
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glNormal3d(0.0, 0.0, 1.0   );
  glVertex3d(0.0, 0.0, height);
  for (j=slices; j>=0; j--)
    glVertex3d(cost[j]*radius, sint[j]*radius, height);
  glEnd();

  /* Do the stacks */

  z0 = 0.0;
  z1 = zStep;

  for (i=1; i<=stacks; i++)
  {
    if (i==stacks)
      z1 = height;

    glBegin(GL_QUAD_STRIP);
    for (j=0; j<=slices; j++ )
    {
      glNormal3d(cost[j],        sint[j],        0.0 );
      glVertex3d(cost[j]*radius, sint[j]*radius, z0  );
      glVertex3d(cost[j]*radius, sint[j]*radius, z1  );
    }
    glEnd();

    z0 = z1; z1 += zStep;
  }

  /* Release sin and cos tables */

  free(sint);
  free(cost);
}

/*
* Draws a wire cylinder
*/
void  glutWireCylinder(float radius, float height, GLint slices, GLint stacks)
{
  int i,j;

  /* Step in z and radius as stacks are drawn. */

  double z = 0.0;
  const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );

  /* Pre-computed circle */

  double *sint,*cost;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutWireCylinder" );

  fghCircleTable(&sint,&cost,-slices);

  /* Draw the stacks... */

  for (i=0; i<=stacks; i++)
  {
    if (i==stacks)
      z = height;

    glBegin(GL_LINE_LOOP);

    for( j=0; j<slices; j++ )
    {
      glNormal3d(cost[j],        sint[j],        0.0);
      glVertex3d(cost[j]*radius, sint[j]*radius, z  );
    }

    glEnd();

    z += zStep;
  }

  /* Draw the slices */

  glBegin(GL_LINES);

  for (j=0; j<slices; j++)
  {
    glNormal3d(cost[j],        sint[j],        0.0   );
    glVertex3d(cost[j]*radius, sint[j]*radius, 0.0   );
    glVertex3d(cost[j]*radius, sint[j]*radius, height);
  }

  glEnd();

  /* Release sin and cos tables */

  free(sint);
  free(cost);
}

/*
* Draws a wire torus
*/
void  glutWireTorus( float dInnerRadius, float dOuterRadius, GLint nSides, GLint nRings )
{
  double  iradius = dInnerRadius, oradius = dOuterRadius, phi, psi, dpsi, dphi;
  double *vertex, *normal;
  int    i, j;
  double spsi, cpsi, sphi, cphi ;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutWireTorus" );

  if ( nSides < 1 ) nSides = 1;
  if ( nRings < 1 ) nRings = 1;

  /* Allocate the vertices array */
  vertex = (double *)calloc( sizeof(double), 3 * nSides * nRings );
  normal = (double *)calloc( sizeof(double), 3 * nSides * nRings );

  glPushMatrix();

  dpsi =  2.0 * M_PI / (double)nRings ;
  dphi = -2.0 * M_PI / (double)nSides ;
  psi  = 0.0;

  for( j=0; j<nRings; j++ )
  {
    cpsi = cos ( psi ) ;
    spsi = sin ( psi ) ;
    phi = 0.0;

    for( i=0; i<nSides; i++ )
    {
      int offset = 3 * ( j * nSides + i ) ;
      cphi = cos ( phi ) ;
      sphi = sin ( phi ) ;
      *(vertex + offset + 0) = cpsi * ( oradius + cphi * iradius ) ;
      *(vertex + offset + 1) = spsi * ( oradius + cphi * iradius ) ;
      *(vertex + offset + 2) =                    sphi * iradius  ;
      *(normal + offset + 0) = cpsi * cphi ;
      *(normal + offset + 1) = spsi * cphi ;
      *(normal + offset + 2) =        sphi ;
      phi += dphi;
    }

    psi += dpsi;
  }

  for( i=0; i<nSides; i++ )
  {
    glBegin( GL_LINE_LOOP );

    for( j=0; j<nRings; j++ )
    {
      int offset = 3 * ( j * nSides + i ) ;
      glNormal3dv( normal + offset );
      glVertex3dv( vertex + offset );
    }

    glEnd();
  }

  for( j=0; j<nRings; j++ )
  {
    glBegin(GL_LINE_LOOP);

    for( i=0; i<nSides; i++ )
    {
      int offset = 3 * ( j * nSides + i ) ;
      glNormal3dv( normal + offset );
      glVertex3dv( vertex + offset );
    }

    glEnd();
  }

  free ( vertex ) ;
  free ( normal ) ;
  glPopMatrix();
}

/*
* Draws a solid torus
*/
void  glutSolidTorus( float dInnerRadius, float dOuterRadius, GLint nSides, GLint nRings )
{
  double  iradius = dInnerRadius, oradius = dOuterRadius, phi, psi, dpsi, dphi;
  double *vertex, *normal;
  int    i, j;
  double spsi, cpsi, sphi, cphi ;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidTorus" );

  if ( nSides < 1 ) nSides = 1;
  if ( nRings < 1 ) nRings = 1;

  /* Increment the number of sides and rings to allow for one more point than surface */
  nSides ++ ;
  nRings ++ ;

  /* Allocate the vertices array */
  vertex = (double *)calloc( sizeof(double), 3 * nSides * nRings );
  normal = (double *)calloc( sizeof(double), 3 * nSides * nRings );

  glPushMatrix();

  dpsi =  2.0 * M_PI / (double)(nRings - 1) ;
  dphi = -2.0 * M_PI / (double)(nSides - 1) ;
  psi  = 0.0;

  for( j=0; j<nRings; j++ )
  {
    cpsi = cos ( psi ) ;
    spsi = sin ( psi ) ;
    phi = 0.0;

    for( i=0; i<nSides; i++ )
    {
      int offset = 3 * ( j * nSides + i ) ;
      cphi = cos ( phi ) ;
      sphi = sin ( phi ) ;
      *(vertex + offset + 0) = cpsi * ( oradius + cphi * iradius ) ;
      *(vertex + offset + 1) = spsi * ( oradius + cphi * iradius ) ;
      *(vertex + offset + 2) =                    sphi * iradius  ;
      *(normal + offset + 0) = cpsi * cphi ;
      *(normal + offset + 1) = spsi * cphi ;
      *(normal + offset + 2) =        sphi ;
      phi += dphi;
    }

    psi += dpsi;
  }

  glBegin( GL_QUADS );
  for( i=0; i<nSides-1; i++ )
  {
    for( j=0; j<nRings-1; j++ )
    {
      int offset = 3 * ( j * nSides + i ) ;
      glNormal3dv( normal + offset );
      glVertex3dv( vertex + offset );
      glNormal3dv( normal + offset + 3 );
      glVertex3dv( vertex + offset + 3 );
      glNormal3dv( normal + offset + 3 * nSides + 3 );
      glVertex3dv( vertex + offset + 3 * nSides + 3 );
      glNormal3dv( normal + offset + 3 * nSides );
      glVertex3dv( vertex + offset + 3 * nSides );
    }
  }

  glEnd();

  free ( vertex ) ;
  free ( normal ) ;
  glPopMatrix();
}

/*
*
*/
void  glutWireDodecahedron( void )
{
  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutWireDodecahedron" );

  /* Magic Numbers:  It is possible to create a dodecahedron by attaching two pentagons to each face of
  * of a cube.  The coordinates of the points are:
  *   (+-x,0, z); (+-1, 1, 1); (0, z, x )
  * where x = (-1 + sqrt(5))/2, z = (1 + sqrt(5))/2  or
  *       x = 0.61803398875 and z = 1.61803398875.
  */
  glBegin ( GL_LINE_LOOP ) ;
  glNormal3d (  0.0,  0.525731112119,  0.850650808354 ) ; glVertex3d (  0.0,  1.61803398875,  0.61803398875 ) ; glVertex3d ( -1.0,  1.0,  1.0 ) ; glVertex3d ( -0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d (  0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d (  1.0,  1.0,  1.0 ) ;
  glEnd () ;
  glBegin ( GL_LINE_LOOP ) ;
  glNormal3d (  0.0,  0.525731112119, -0.850650808354 ) ; glVertex3d (  0.0,  1.61803398875, -0.61803398875 ) ; glVertex3d (  1.0,  1.0, -1.0 ) ; glVertex3d (  0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d ( -0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d ( -1.0,  1.0, -1.0 ) ;
  glEnd () ;
  glBegin ( GL_LINE_LOOP ) ;
  glNormal3d (  0.0, -0.525731112119,  0.850650808354 ) ; glVertex3d (  0.0, -1.61803398875,  0.61803398875 ) ; glVertex3d (  1.0, -1.0,  1.0 ) ; glVertex3d (  0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d ( -0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d ( -1.0, -1.0,  1.0 ) ;
  glEnd () ;
  glBegin ( GL_LINE_LOOP ) ;
  glNormal3d (  0.0, -0.525731112119, -0.850650808354 ) ; glVertex3d (  0.0, -1.61803398875, -0.61803398875 ) ; glVertex3d ( -1.0, -1.0, -1.0 ) ; glVertex3d ( -0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d (  0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d (  1.0, -1.0, -1.0 ) ;
  glEnd () ;

  glBegin ( GL_LINE_LOOP ) ;
  glNormal3d (  0.850650808354,  0.0,  0.525731112119 ) ; glVertex3d (  0.61803398875,  0.0,  1.61803398875 ) ; glVertex3d (  1.0, -1.0,  1.0 ) ; glVertex3d (  1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d (  1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d (  1.0,  1.0,  1.0 ) ;
  glEnd () ;
  glBegin ( GL_LINE_LOOP ) ;
  glNormal3d ( -0.850650808354,  0.0,  0.525731112119 ) ; glVertex3d ( -0.61803398875,  0.0,  1.61803398875 ) ; glVertex3d ( -1.0,  1.0,  1.0 ) ; glVertex3d ( -1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d ( -1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d ( -1.0, -1.0,  1.0 ) ;
  glEnd () ;
  glBegin ( GL_LINE_LOOP ) ;
  glNormal3d (  0.850650808354,  0.0, -0.525731112119 ) ; glVertex3d (  0.61803398875,  0.0, -1.61803398875 ) ; glVertex3d (  1.0,  1.0, -1.0 ) ; glVertex3d (  1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d (  1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d (  1.0, -1.0, -1.0 ) ;
  glEnd () ;
  glBegin ( GL_LINE_LOOP ) ;
  glNormal3d ( -0.850650808354,  0.0, -0.525731112119 ) ; glVertex3d ( -0.61803398875,  0.0, -1.61803398875 ) ; glVertex3d ( -1.0, -1.0, -1.0 ) ; glVertex3d ( -1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d ( -1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d ( -1.0,  1.0, -1.0 ) ;
  glEnd () ;

  glBegin ( GL_LINE_LOOP ) ;
  glNormal3d (  0.525731112119,  0.850650808354,  0.0 ) ; glVertex3d (  1.61803398875,  0.61803398875,  0.0 ) ; glVertex3d (  1.0,  1.0, -1.0 ) ; glVertex3d ( 0.0,  1.61803398875, -0.61803398875 ) ; glVertex3d ( 0.0,  1.61803398875,  0.61803398875 ) ; glVertex3d (  1.0,  1.0,  1.0 ) ;
  glEnd () ;
  glBegin ( GL_LINE_LOOP ) ;
  glNormal3d (  0.525731112119, -0.850650808354,  0.0 ) ; glVertex3d (  1.61803398875, -0.61803398875,  0.0 ) ; glVertex3d (  1.0, -1.0,  1.0 ) ; glVertex3d ( 0.0, -1.61803398875,  0.61803398875 ) ; glVertex3d ( 0.0, -1.61803398875, -0.61803398875 ) ; glVertex3d (  1.0, -1.0, -1.0 ) ;
  glEnd () ;
  glBegin ( GL_LINE_LOOP ) ;
  glNormal3d ( -0.525731112119,  0.850650808354,  0.0 ) ; glVertex3d ( -1.61803398875,  0.61803398875,  0.0 ) ; glVertex3d ( -1.0,  1.0,  1.0 ) ; glVertex3d ( 0.0,  1.61803398875,  0.61803398875 ) ; glVertex3d ( 0.0,  1.61803398875, -0.61803398875 ) ; glVertex3d ( -1.0,  1.0, -1.0 ) ;
  glEnd () ;
  glBegin ( GL_LINE_LOOP ) ;
  glNormal3d ( -0.525731112119, -0.850650808354,  0.0 ) ; glVertex3d ( -1.61803398875, -0.61803398875,  0.0 ) ; glVertex3d ( -1.0, -1.0, -1.0 ) ; glVertex3d ( 0.0, -1.61803398875, -0.61803398875 ) ; glVertex3d ( 0.0, -1.61803398875,  0.61803398875 ) ; glVertex3d ( -1.0, -1.0,  1.0 ) ;
  glEnd () ;
}

/*
*
*/
void  glutSolidDodecahedron( void )
{
  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidDodecahedron" );

  /* Magic Numbers:  It is possible to create a dodecahedron by attaching two pentagons to each face of
  * of a cube.  The coordinates of the points are:
  *   (+-x,0, z); (+-1, 1, 1); (0, z, x )
  * where x = (-1 + sqrt(5))/2, z = (1 + sqrt(5))/2 or
  *       x = 0.61803398875 and z = 1.61803398875.
  */
  glBegin ( GL_POLYGON ) ;
  glNormal3d (  0.0,  0.525731112119,  0.850650808354 ) ; glVertex3d (  0.0,  1.61803398875,  0.61803398875 ) ; glVertex3d ( -1.0,  1.0,  1.0 ) ; glVertex3d ( -0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d (  0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d (  1.0,  1.0,  1.0 ) ;
  glEnd () ;
  glBegin ( GL_POLYGON ) ;
  glNormal3d (  0.0,  0.525731112119, -0.850650808354 ) ; glVertex3d (  0.0,  1.61803398875, -0.61803398875 ) ; glVertex3d (  1.0,  1.0, -1.0 ) ; glVertex3d (  0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d ( -0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d ( -1.0,  1.0, -1.0 ) ;
  glEnd () ;
  glBegin ( GL_POLYGON ) ;
  glNormal3d (  0.0, -0.525731112119,  0.850650808354 ) ; glVertex3d (  0.0, -1.61803398875,  0.61803398875 ) ; glVertex3d (  1.0, -1.0,  1.0 ) ; glVertex3d (  0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d ( -0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d ( -1.0, -1.0,  1.0 ) ;
  glEnd () ;
  glBegin ( GL_POLYGON ) ;
  glNormal3d (  0.0, -0.525731112119, -0.850650808354 ) ; glVertex3d (  0.0, -1.61803398875, -0.61803398875 ) ; glVertex3d ( -1.0, -1.0, -1.0 ) ; glVertex3d ( -0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d (  0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d (  1.0, -1.0, -1.0 ) ;
  glEnd () ;

  glBegin ( GL_POLYGON ) ;
  glNormal3d (  0.850650808354,  0.0,  0.525731112119 ) ; glVertex3d (  0.61803398875,  0.0,  1.61803398875 ) ; glVertex3d (  1.0, -1.0,  1.0 ) ; glVertex3d (  1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d (  1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d (  1.0,  1.0,  1.0 ) ;
  glEnd () ;
  glBegin ( GL_POLYGON ) ;
  glNormal3d ( -0.850650808354,  0.0,  0.525731112119 ) ; glVertex3d ( -0.61803398875,  0.0,  1.61803398875 ) ; glVertex3d ( -1.0,  1.0,  1.0 ) ; glVertex3d ( -1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d ( -1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d ( -1.0, -1.0,  1.0 ) ;
  glEnd () ;
  glBegin ( GL_POLYGON ) ;
  glNormal3d (  0.850650808354,  0.0, -0.525731112119 ) ; glVertex3d (  0.61803398875,  0.0, -1.61803398875 ) ; glVertex3d (  1.0,  1.0, -1.0 ) ; glVertex3d (  1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d (  1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d (  1.0, -1.0, -1.0 ) ;
  glEnd () ;
  glBegin ( GL_POLYGON ) ;
  glNormal3d ( -0.850650808354,  0.0, -0.525731112119 ) ; glVertex3d ( -0.61803398875,  0.0, -1.61803398875 ) ; glVertex3d ( -1.0, -1.0, -1.0 ) ; glVertex3d ( -1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d ( -1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d ( -1.0,  1.0, -1.0 ) ;
  glEnd () ;

  glBegin ( GL_POLYGON ) ;
  glNormal3d (  0.525731112119,  0.850650808354,  0.0 ) ; glVertex3d (  1.61803398875,  0.61803398875,  0.0 ) ; glVertex3d (  1.0,  1.0, -1.0 ) ; glVertex3d ( 0.0,  1.61803398875, -0.61803398875 ) ; glVertex3d ( 0.0,  1.61803398875,  0.61803398875 ) ; glVertex3d (  1.0,  1.0,  1.0 ) ;
  glEnd () ;
  glBegin ( GL_POLYGON ) ;
  glNormal3d (  0.525731112119, -0.850650808354,  0.0 ) ; glVertex3d (  1.61803398875, -0.61803398875,  0.0 ) ; glVertex3d (  1.0, -1.0,  1.0 ) ; glVertex3d ( 0.0, -1.61803398875,  0.61803398875 ) ; glVertex3d ( 0.0, -1.61803398875, -0.61803398875 ) ; glVertex3d (  1.0, -1.0, -1.0 ) ;
  glEnd () ;
  glBegin ( GL_POLYGON ) ;
  glNormal3d ( -0.525731112119,  0.850650808354,  0.0 ) ; glVertex3d ( -1.61803398875,  0.61803398875,  0.0 ) ; glVertex3d ( -1.0,  1.0,  1.0 ) ; glVertex3d ( 0.0,  1.61803398875,  0.61803398875 ) ; glVertex3d ( 0.0,  1.61803398875, -0.61803398875 ) ; glVertex3d ( -1.0,  1.0, -1.0 ) ;
  glEnd () ;
  glBegin ( GL_POLYGON ) ;
  glNormal3d ( -0.525731112119, -0.850650808354,  0.0 ) ; glVertex3d ( -1.61803398875, -0.61803398875,  0.0 ) ; glVertex3d ( -1.0, -1.0, -1.0 ) ; glVertex3d ( 0.0, -1.61803398875, -0.61803398875 ) ; glVertex3d ( 0.0, -1.61803398875,  0.61803398875 ) ; glVertex3d ( -1.0, -1.0,  1.0 ) ;
  glEnd () ;
}

/*
*
*/
void  glutWireOctahedron( void )
{
  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutWireOctahedron" );

#define RADIUS    1.0f
  glBegin( GL_LINE_LOOP );
  glNormal3d( 0.577350269189, 0.577350269189, 0.577350269189); glVertex3d( RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, RADIUS, 0.0 ); glVertex3d( 0.0, 0.0, RADIUS );
  glNormal3d( 0.577350269189, 0.577350269189,-0.577350269189); glVertex3d( RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, 0.0,-RADIUS ); glVertex3d( 0.0, RADIUS, 0.0 );
  glNormal3d( 0.577350269189,-0.577350269189, 0.577350269189); glVertex3d( RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, 0.0, RADIUS ); glVertex3d( 0.0,-RADIUS, 0.0 );
  glNormal3d( 0.577350269189,-0.577350269189,-0.577350269189); glVertex3d( RADIUS, 0.0, 0.0 ); glVertex3d( 0.0,-RADIUS, 0.0 ); glVertex3d( 0.0, 0.0,-RADIUS );
  glNormal3d(-0.577350269189, 0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, 0.0, RADIUS ); glVertex3d( 0.0, RADIUS, 0.0 );
  glNormal3d(-0.577350269189, 0.577350269189,-0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, RADIUS, 0.0 ); glVertex3d( 0.0, 0.0,-RADIUS );
  glNormal3d(-0.577350269189,-0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0 ); glVertex3d( 0.0,-RADIUS, 0.0 ); glVertex3d( 0.0, 0.0, RADIUS );
  glNormal3d(-0.577350269189,-0.577350269189,-0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, 0.0,-RADIUS ); glVertex3d( 0.0,-RADIUS, 0.0 );
  glEnd();
#undef RADIUS
}

/*
*
*/
void  glutSolidOctahedron( void )
{
  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidOctahedron" );

#define RADIUS    1.0f
  glBegin( GL_TRIANGLES );
  glNormal3d( 0.577350269189, 0.577350269189, 0.577350269189); glVertex3d( RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, RADIUS, 0.0 ); glVertex3d( 0.0, 0.0, RADIUS );
  glNormal3d( 0.577350269189, 0.577350269189,-0.577350269189); glVertex3d( RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, 0.0,-RADIUS ); glVertex3d( 0.0, RADIUS, 0.0 );
  glNormal3d( 0.577350269189,-0.577350269189, 0.577350269189); glVertex3d( RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, 0.0, RADIUS ); glVertex3d( 0.0,-RADIUS, 0.0 );
  glNormal3d( 0.577350269189,-0.577350269189,-0.577350269189); glVertex3d( RADIUS, 0.0, 0.0 ); glVertex3d( 0.0,-RADIUS, 0.0 ); glVertex3d( 0.0, 0.0,-RADIUS );
  glNormal3d(-0.577350269189, 0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, 0.0, RADIUS ); glVertex3d( 0.0, RADIUS, 0.0 );
  glNormal3d(-0.577350269189, 0.577350269189,-0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, RADIUS, 0.0 ); glVertex3d( 0.0, 0.0,-RADIUS );
  glNormal3d(-0.577350269189,-0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0 ); glVertex3d( 0.0,-RADIUS, 0.0 ); glVertex3d( 0.0, 0.0, RADIUS );
  glNormal3d(-0.577350269189,-0.577350269189,-0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, 0.0,-RADIUS ); glVertex3d( 0.0,-RADIUS, 0.0 );
  glEnd();
#undef RADIUS
}

/* Magic Numbers:  r0 = ( 1, 0, 0 )
*                 r1 = ( -1/3, 2 sqrt(2) / 3, 0 )
*                 r2 = ( -1/3, -sqrt(2) / 3, sqrt(6) / 3 )
*                 r3 = ( -1/3, -sqrt(2) / 3, -sqrt(6) / 3 )
* |r0| = |r1| = |r2| = |r3| = 1
* Distance between any two points is 2 sqrt(6) / 3
*
* Normals:  The unit normals are simply the negative of the coordinates of the point not on the surface.
*/

#define NUM_TETR_FACES     4

static float tet_r[4][3] = { {             1.0,             0.0,             0.0 },
{ -0.333333333333,  0.942809041582,             0.0 },
{ -0.333333333333, -0.471404520791,  0.816496580928 },
{ -0.333333333333, -0.471404520791, -0.816496580928 } } ;

static GLint tet_i[4][3] =  /* Vertex indices */
{
  { 1, 3, 2 }, { 0, 2, 3 }, { 0, 3, 1 }, { 0, 1, 2 }
} ;

/*
*
*/
void  glutWireTetrahedron( void )
{
  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutWireTetrahedron" );

  glBegin( GL_LINE_LOOP ) ;
  glNormal3d ( -tet_r[0][0], -tet_r[0][1], -tet_r[0][2] ) ; glVertex3dv ( tet_r[1] ) ; glVertex3dv ( tet_r[3] ) ; glVertex3dv ( tet_r[2] ) ;
  glNormal3d ( -tet_r[1][0], -tet_r[1][1], -tet_r[1][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[2] ) ; glVertex3dv ( tet_r[3] ) ;
  glNormal3d ( -tet_r[2][0], -tet_r[2][1], -tet_r[2][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[3] ) ; glVertex3dv ( tet_r[1] ) ;
  glNormal3d ( -tet_r[3][0], -tet_r[3][1], -tet_r[3][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[1] ) ; glVertex3dv ( tet_r[2] ) ;
  glEnd() ;
}

/*
*
*/
void  glutSolidTetrahedron( void )
{
  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidTetrahedron" );

  glBegin( GL_TRIANGLES ) ;
  glNormal3d ( -tet_r[0][0], -tet_r[0][1], -tet_r[0][2] ) ; glVertex3dv ( tet_r[1] ) ; glVertex3dv ( tet_r[3] ) ; glVertex3dv ( tet_r[2] ) ;
  glNormal3d ( -tet_r[1][0], -tet_r[1][1], -tet_r[1][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[2] ) ; glVertex3dv ( tet_r[3] ) ;
  glNormal3d ( -tet_r[2][0], -tet_r[2][1], -tet_r[2][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[3] ) ; glVertex3dv ( tet_r[1] ) ;
  glNormal3d ( -tet_r[3][0], -tet_r[3][1], -tet_r[3][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[1] ) ; glVertex3dv ( tet_r[2] ) ;
  glEnd() ;
}

/*
*
*/
static double icos_r[12][3] = {
  {  1.0,             0.0,             0.0            },
  {  0.447213595500,  0.894427191000,  0.0            },
  {  0.447213595500,  0.276393202252,  0.850650808354 },
  {  0.447213595500, -0.723606797748,  0.525731112119 },
  {  0.447213595500, -0.723606797748, -0.525731112119 },
  {  0.447213595500,  0.276393202252, -0.850650808354 },
  { -0.447213595500, -0.894427191000,  0.0 },
  { -0.447213595500, -0.276393202252,  0.850650808354 },
  { -0.447213595500,  0.723606797748,  0.525731112119 },
  { -0.447213595500,  0.723606797748, -0.525731112119 },
  { -0.447213595500, -0.276393202252, -0.850650808354 },
  { -1.0,             0.0,             0.0            }
};

static int icos_v [20][3] = {
  {  0,  1,  2 },
  {  0,  2,  3 },
  {  0,  3,  4 },
  {  0,  4,  5 },
  {  0,  5,  1 },
  {  1,  8,  2 },
  {  2,  7,  3 },
  {  3,  6,  4 },
  {  4, 10,  5 },
  {  5,  9,  1 },
  {  1,  9,  8 },
  {  2,  8,  7 },
  {  3,  7,  6 },
  {  4,  6, 10 },
  {  5, 10,  9 },
  { 11,  9, 10 },
  { 11,  8,  9 },
  { 11,  7,  8 },
  { 11,  6,  7 },
  { 11, 10,  6 }
};

void  glutWireIcosahedron( void )
{
  int i ;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutWireIcosahedron" );

  for ( i = 0; i < 20; i++ )
  {
    double normal[3] ;
    normal[0] = ( icos_r[icos_v[i][1]][1] - icos_r[icos_v[i][0]][1] ) * ( icos_r[icos_v[i][2]][2] - icos_r[icos_v[i][0]][2] ) - ( icos_r[icos_v[i][1]][2] - icos_r[icos_v[i][0]][2] ) * ( icos_r[icos_v[i][2]][1] - icos_r[icos_v[i][0]][1] ) ;
    normal[1] = ( icos_r[icos_v[i][1]][2] - icos_r[icos_v[i][0]][2] ) * ( icos_r[icos_v[i][2]][0] - icos_r[icos_v[i][0]][0] ) - ( icos_r[icos_v[i][1]][0] - icos_r[icos_v[i][0]][0] ) * ( icos_r[icos_v[i][2]][2] - icos_r[icos_v[i][0]][2] ) ;
    normal[2] = ( icos_r[icos_v[i][1]][0] - icos_r[icos_v[i][0]][0] ) * ( icos_r[icos_v[i][2]][1] - icos_r[icos_v[i][0]][1] ) - ( icos_r[icos_v[i][1]][1] - icos_r[icos_v[i][0]][1] ) * ( icos_r[icos_v[i][2]][0] - icos_r[icos_v[i][0]][0] ) ;
    glBegin ( GL_LINE_LOOP ) ;
    glNormal3dv ( normal ) ;
    glVertex3dv ( icos_r[icos_v[i][0]] ) ;
    glVertex3dv ( icos_r[icos_v[i][1]] ) ;
    glVertex3dv ( icos_r[icos_v[i][2]] ) ;
    glEnd () ;
  }
}

/*
*
*/
void  glutSolidIcosahedron( void )
{
  int i ;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidIcosahedron" );

  glBegin ( GL_TRIANGLES ) ;
  for ( i = 0; i < 20; i++ )
  {
    double normal[3] ;
    normal[0] = ( icos_r[icos_v[i][1]][1] - icos_r[icos_v[i][0]][1] ) * ( icos_r[icos_v[i][2]][2] - icos_r[icos_v[i][0]][2] ) - ( icos_r[icos_v[i][1]][2] - icos_r[icos_v[i][0]][2] ) * ( icos_r[icos_v[i][2]][1] - icos_r[icos_v[i][0]][1] ) ;
    normal[1] = ( icos_r[icos_v[i][1]][2] - icos_r[icos_v[i][0]][2] ) * ( icos_r[icos_v[i][2]][0] - icos_r[icos_v[i][0]][0] ) - ( icos_r[icos_v[i][1]][0] - icos_r[icos_v[i][0]][0] ) * ( icos_r[icos_v[i][2]][2] - icos_r[icos_v[i][0]][2] ) ;
    normal[2] = ( icos_r[icos_v[i][1]][0] - icos_r[icos_v[i][0]][0] ) * ( icos_r[icos_v[i][2]][1] - icos_r[icos_v[i][0]][1] ) - ( icos_r[icos_v[i][1]][1] - icos_r[icos_v[i][0]][1] ) * ( icos_r[icos_v[i][2]][0] - icos_r[icos_v[i][0]][0] ) ;
    glNormal3dv ( normal ) ;
    glVertex3dv ( icos_r[icos_v[i][0]] ) ;
    glVertex3dv ( icos_r[icos_v[i][1]] ) ;
    glVertex3dv ( icos_r[icos_v[i][2]] ) ;
  }

  glEnd () ;
}

/*
*
*/
static double rdod_r[14][3] = {
  {  0.0,             0.0,             1.0 },
  {  0.707106781187,  0.000000000000,  0.5 },
  {  0.000000000000,  0.707106781187,  0.5 },
  { -0.707106781187,  0.000000000000,  0.5 },
  {  0.000000000000, -0.707106781187,  0.5 },
  {  0.707106781187,  0.707106781187,  0.0 },
  { -0.707106781187,  0.707106781187,  0.0 },
  { -0.707106781187, -0.707106781187,  0.0 },
  {  0.707106781187, -0.707106781187,  0.0 },
  {  0.707106781187,  0.000000000000, -0.5 },
  {  0.000000000000,  0.707106781187, -0.5 },
  { -0.707106781187,  0.000000000000, -0.5 },
  {  0.000000000000, -0.707106781187, -0.5 },
  {  0.0,             0.0,            -1.0 }
} ;

static int rdod_v [12][4] = {
  { 0,  1,  5,  2 },
  { 0,  2,  6,  3 },
  { 0,  3,  7,  4 },
  { 0,  4,  8,  1 },
  { 5, 10,  6,  2 },
  { 6, 11,  7,  3 },
  { 7, 12,  8,  4 },
  { 8,  9,  5,  1 },
  { 5,  9, 13, 10 },
  { 6, 10, 13, 11 },
  { 7, 11, 13, 12 },
  { 8, 12, 13,  9 }
};

static double rdod_n[12][3] = {
  {  0.353553390594,  0.353553390594,  0.5 },
  { -0.353553390594,  0.353553390594,  0.5 },
  { -0.353553390594, -0.353553390594,  0.5 },
  {  0.353553390594, -0.353553390594,  0.5 },
  {  0.000000000000,  1.000000000000,  0.0 },
  { -1.000000000000,  0.000000000000,  0.0 },
  {  0.000000000000, -1.000000000000,  0.0 },
  {  1.000000000000,  0.000000000000,  0.0 },
  {  0.353553390594,  0.353553390594, -0.5 },
  { -0.353553390594,  0.353553390594, -0.5 },
  { -0.353553390594, -0.353553390594, -0.5 },
  {  0.353553390594, -0.353553390594, -0.5 }
};

void  glutWireRhombicDodecahedron( void )
{
  int i ;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutWireRhombicDodecahedron" );

  for ( i = 0; i < 12; i++ )
  {
    glBegin ( GL_LINE_LOOP ) ;
    glNormal3dv ( rdod_n[i] ) ;
    glVertex3dv ( rdod_r[rdod_v[i][0]] ) ;
    glVertex3dv ( rdod_r[rdod_v[i][1]] ) ;
    glVertex3dv ( rdod_r[rdod_v[i][2]] ) ;
    glVertex3dv ( rdod_r[rdod_v[i][3]] ) ;
    glEnd () ;
  }
}

/*
*
*/
void  glutSolidRhombicDodecahedron( void )
{
  int i ;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidRhombicDodecahedron" );

  glBegin ( GL_QUADS ) ;
  for ( i = 0; i < 12; i++ )
  {
    glNormal3dv ( rdod_n[i] ) ;
    glVertex3dv ( rdod_r[rdod_v[i][0]] ) ;
    glVertex3dv ( rdod_r[rdod_v[i][1]] ) ;
    glVertex3dv ( rdod_r[rdod_v[i][2]] ) ;
    glVertex3dv ( rdod_r[rdod_v[i][3]] ) ;
  }

  glEnd () ;
}

void  glutWireSierpinskiSponge ( int num_levels, float offset[3], float scale )
{
  int i, j ;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutWireSierpinskiSponge" );

  if ( num_levels == 0 )
  {

    for ( i = 0 ; i < NUM_TETR_FACES ; i++ )
    {
      glBegin ( GL_LINE_LOOP ) ;
      glNormal3d ( -tet_r[i][0], -tet_r[i][1], -tet_r[i][2] ) ;
      for ( j = 0; j < 3; j++ )
      {
        double x = offset[0] + scale * tet_r[tet_i[i][j]][0] ;
        double y = offset[1] + scale * tet_r[tet_i[i][j]][1] ;
        double z = offset[2] + scale * tet_r[tet_i[i][j]][2] ;
        glVertex3d ( x, y, z ) ;
      }

      glEnd () ;
    }
  }
  else if ( num_levels > 0 )
  {
    float local_offset[3] ;  /* Use a local variable to avoid buildup of roundoff errors */
    num_levels -- ;
    scale /= 2.0 ;
    for ( i = 0 ; i < NUM_TETR_FACES ; i++ )
    {
      local_offset[0] = offset[0] + scale * tet_r[i][0] ;
      local_offset[1] = offset[1] + scale * tet_r[i][1] ;
      local_offset[2] = offset[2] + scale * tet_r[i][2] ;
      glutWireSierpinskiSponge ( num_levels, local_offset, scale ) ;
    }
  }
}

void  glutSolidSierpinskiSponge ( int num_levels, float offset[3], float scale )
{
  int i, j ;

  FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidSierpinskiSponge" );

  if ( num_levels == 0 )
  {
    glBegin ( GL_TRIANGLES ) ;

    for ( i = 0 ; i < NUM_TETR_FACES ; i++ )
    {
      glNormal3d ( -tet_r[i][0], -tet_r[i][1], -tet_r[i][2] ) ;
      for ( j = 0; j < 3; j++ )
      {
        double x = offset[0] + scale * tet_r[tet_i[i][j]][0] ;
        double y = offset[1] + scale * tet_r[tet_i[i][j]][1] ;
        double z = offset[2] + scale * tet_r[tet_i[i][j]][2] ;
        glVertex3d ( x, y, z ) ;
      }
    }

    glEnd () ;
  }
  else if ( num_levels > 0 )
  {
    float local_offset[3] ;  /* Use a local variable to avoid buildup of roundoff errors */
    num_levels -- ;
    scale /= 2.0 ;
    for ( i = 0 ; i < NUM_TETR_FACES ; i++ )
    {
      local_offset[0] = offset[0] + scale * tet_r[i][0] ;
      local_offset[1] = offset[1] + scale * tet_r[i][1] ;
      local_offset[2] = offset[2] + scale * tet_r[i][2] ;
      glutSolidSierpinskiSponge ( num_levels, local_offset, scale ) ;
    }
  }
}
#endif
}