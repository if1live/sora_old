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

namespace sora {;

#if 0
ModelType PrimitiveModel::type() const { return ModelTypePrimitive; }

VertexPointer SolidPrimitiveModel::vertexPointer() const
{
  SR_ASSERT(vertexList_.size() > 0);

  const float *pos = &vertexList_[0].pos[0];
  const float *color = &vertexList_[0].color[0];
  const float *texCoord = &vertexList_[0].texCoord[0];
  const float *normal = &vertexList_[0].normal[0];

  VertexPointerBuilder builder;
  builder.setStartPointer(pos);
  builder.setVertexOffset(0);

  builder.setColorOffset(builder.calcOffset<float>(color, pos));
  builder.setTexCoordOffset(builder.calcOffset<float>(texCoord, pos));
  builder.setNormalOffset(builder.calcOffset<float>(normal, pos));
  builder.setStride(sizeof(Vertex3));
  builder.setCount(vertexList_.size());

  VertexPointer vp;
  builder.build(vp);
  return vp;
}

VertexPointer WirePrimitiveModel::vertexPointer() const
{
  SR_ASSERT(vertexList_.size() > 0);

  //포인터의 뺼셈을 그냥수행하면 원래 타입으로 몇개가 들어가는 자리인지가 뜨는데
  //실제로 엔진에서 필요한 정보는 바이트로써의 차이다. 그래서 캐스팅함
  const float *pos = &vertexList_[0].pos[0];
  const float *color = &vertexList_[0].color[0];

  VertexPointerBuilder builder;
  builder.setStartPointer(pos);
  builder.setVertexOffset(0);
  builder.setColorOffset(builder.calcOffset<float>(color, pos));
  builder.setCount(vertexList_.size());
  builder.setStride(sizeof(ColorVertex3));

  VertexPointer vp;
  builder.build(vp);
  return vp;
}
#if _IOS_
#pragma mark solid cube
#endif
SolidCube::SolidCube(GLfloat width, GLfloat height, GLfloat depth, const matsu::vec4 &color)
  : SolidPrimitiveModel()
{
  assert(width > 0 && height > 0 && depth > 0);
  width = width/2;
  height = height/2;
  depth = depth/2;

  //construct vertex list
  //GL_TRIANGLES용 index list도 같이 구성한다
  //ccw base

  matsu::vec4 colorVec(color);
  {
    // Front Face
    int baseIndex = vertexList_.size();

    matsu::vec3 normal(0, 0, 1);	// Normal Pointing Towards Viewer

    matsu::vec2 texCoord1(0, 0);	matsu::vec3 vertex1(-width, -height, depth);
    matsu::vec2 texCoord2(1, 0);	matsu::vec3 vertex2( width, -height, depth);
    matsu::vec2 texCoord3(1, 1);	matsu::vec3 vertex3( width,  height, depth);
    matsu::vec2 texCoord4(0, 1);	matsu::vec3 vertex4(-width,  height, depth);

    //add vertex
    Vertex3 v1(vertex1, colorVec, texCoord1, normal);
    Vertex3 v2(vertex2, colorVec, texCoord2, normal);
    Vertex3 v3(vertex3, colorVec, texCoord3, normal);
    Vertex3 v4(vertex4, colorVec, texCoord4, normal);

    vertexList_.push_back(v1);
    vertexList_.push_back(v2);
    vertexList_.push_back(v3);
    vertexList_.push_back(v4);

    //add index
    indexList_.push_back(baseIndex + 0);
    indexList_.push_back(baseIndex + 1);
    indexList_.push_back(baseIndex + 2);

    indexList_.push_back(baseIndex + 0);
    indexList_.push_back(baseIndex + 2);
    indexList_.push_back(baseIndex + 3);
  }
  {
    // Back Face
    int baseIndex = vertexList_.size();
    matsu::vec3 normal(0, 0, -1);	// Normal Pointing Away From Viewer

    matsu::vec2 texCoord1(1, 0);	matsu::vec3 vertex1(-width, -height, -depth);
    matsu::vec2 texCoord2(1, 1);	matsu::vec3 vertex2(-width,  height, -depth);
    matsu::vec2 texCoord3(0, 1);	matsu::vec3 vertex3( width,  height, -depth);
    matsu::vec2 texCoord4(0, 0);	matsu::vec3 vertex4( width, -height, -depth);

    //add vertex
    Vertex3 v1(vertex1, colorVec, texCoord1, normal);
    Vertex3 v2(vertex2, colorVec, texCoord2, normal);
    Vertex3 v3(vertex3, colorVec, texCoord3, normal);
    Vertex3 v4(vertex4, colorVec, texCoord4, normal);

    vertexList_.push_back(v1);
    vertexList_.push_back(v2);
    vertexList_.push_back(v3);
    vertexList_.push_back(v4);

    //add index
    indexList_.push_back(baseIndex + 0);
    indexList_.push_back(baseIndex + 1);
    indexList_.push_back(baseIndex + 2);

    indexList_.push_back(baseIndex + 0);
    indexList_.push_back(baseIndex + 2);
    indexList_.push_back(baseIndex + 3);
  }

  {
    // Top Face
    int baseIndex = vertexList_.size();
    matsu::vec3 normal(0, 1, 0);	// Normal Pointing Up

    matsu::vec2 texCoord1(0, 1);	matsu::vec3 vertex1(-width, height, -depth);
    matsu::vec2 texCoord2(0, 0);	matsu::vec3 vertex2(-width, height,  depth);
    matsu::vec2 texCoord3(1, 0);	matsu::vec3 vertex3( width, height,  depth);
    matsu::vec2 texCoord4(1, 1);	matsu::vec3 vertex4( width, height, -depth);

    //add vertex
    Vertex3 v1(vertex1, colorVec, texCoord1, normal);
    Vertex3 v2(vertex2, colorVec, texCoord2, normal);
    Vertex3 v3(vertex3, colorVec, texCoord3, normal);
    Vertex3 v4(vertex4, colorVec, texCoord4, normal);

    vertexList_.push_back(v1);
    vertexList_.push_back(v2);
    vertexList_.push_back(v3);
    vertexList_.push_back(v4);

    //add index
    indexList_.push_back(baseIndex + 0);
    indexList_.push_back(baseIndex + 1);
    indexList_.push_back(baseIndex + 2);

    indexList_.push_back(baseIndex + 0);
    indexList_.push_back(baseIndex + 2);
    indexList_.push_back(baseIndex + 3);
  }

  {
    // Bottom Face
    int baseIndex = vertexList_.size();
    matsu::vec3 normal(0, -1, 0);	// Normal Pointing Down

    matsu::vec2 texCoord1(1, 1);	matsu::vec3 vertex1(-width, -height, -depth);
    matsu::vec2 texCoord2(0, 1);	matsu::vec3 vertex2( width, -height, -depth);
    matsu::vec2 texCoord3(0, 0);	matsu::vec3 vertex3( width, -height,  depth);
    matsu::vec2 texCoord4(1, 0);	matsu::vec3 vertex4(-width, -height,  depth);

    //add vertex
    Vertex3 v1(vertex1, colorVec, texCoord1, normal);
    Vertex3 v2(vertex2, colorVec, texCoord2, normal);
    Vertex3 v3(vertex3, colorVec, texCoord3, normal);
    Vertex3 v4(vertex4, colorVec, texCoord4, normal);

    vertexList_.push_back(v1);
    vertexList_.push_back(v2);
    vertexList_.push_back(v3);
    vertexList_.push_back(v4);

    //add index
    indexList_.push_back(baseIndex + 0);
    indexList_.push_back(baseIndex + 1);
    indexList_.push_back(baseIndex + 2);

    indexList_.push_back(baseIndex + 0);
    indexList_.push_back(baseIndex + 2);
    indexList_.push_back(baseIndex + 3);
  }

  {
    // Right face
    int baseIndex = vertexList_.size();
    matsu::vec3 normal(1, 0, 0);	// Normal Pointing Right

    matsu::vec2 texCoord1(1, 0);	matsu::vec3 vertex1(width, -height, -depth);
    matsu::vec2 texCoord2(1, 1);	matsu::vec3 vertex2(width,  height, -depth);
    matsu::vec2 texCoord3(0, 1);	matsu::vec3 vertex3(width,  height,  depth);
    matsu::vec2 texCoord4(0, 0);	matsu::vec3 vertex4(width, -height,  depth);

    //add vertex
    Vertex3 v1(vertex1, colorVec, texCoord1, normal);
    Vertex3 v2(vertex2, colorVec, texCoord2, normal);
    Vertex3 v3(vertex3, colorVec, texCoord3, normal);
    Vertex3 v4(vertex4, colorVec, texCoord4, normal);

    vertexList_.push_back(v1);
    vertexList_.push_back(v2);
    vertexList_.push_back(v3);
    vertexList_.push_back(v4);

    //add index
    indexList_.push_back(baseIndex + 0);
    indexList_.push_back(baseIndex + 1);
    indexList_.push_back(baseIndex + 2);

    indexList_.push_back(baseIndex + 0);
    indexList_.push_back(baseIndex + 2);
    indexList_.push_back(baseIndex + 3);
  }

  {
    // Left Face
    int baseIndex = vertexList_.size();
    matsu::vec3 normal(-1, 0, 0);	// Normal Pointing Left

    matsu::vec2 texCoord1(0, 0);	matsu::vec3 vertex1(-width, -height, -depth);
    matsu::vec2 texCoord2(1, 0);	matsu::vec3 vertex2(-width, -height,  depth);
    matsu::vec2 texCoord3(1, 1);	matsu::vec3 vertex3(-width,  height,  depth);
    matsu::vec2 texCoord4(0, 1);	matsu::vec3 vertex4(-width,  height, -depth);

    //add vertex
    Vertex3 v1(vertex1, colorVec, texCoord1, normal);
    Vertex3 v2(vertex2, colorVec, texCoord2, normal);
    Vertex3 v3(vertex3, colorVec, texCoord3, normal);
    Vertex3 v4(vertex4, colorVec, texCoord4, normal);

    vertexList_.push_back(v1);
    vertexList_.push_back(v2);
    vertexList_.push_back(v3);
    vertexList_.push_back(v4);

    //add index
    indexList_.push_back(baseIndex + 0);
    indexList_.push_back(baseIndex + 1);
    indexList_.push_back(baseIndex + 2);

    indexList_.push_back(baseIndex + 0);
    indexList_.push_back(baseIndex + 2);
    indexList_.push_back(baseIndex + 3);
  }
}
SolidCube::~SolidCube()
{
}
const std::vector<DrawCommand> SolidCube::getDrawCommand() const
{
  vector<DrawCommand> list;
  DrawCommand cmd(GL_TRIANGLES, indexList_.size(), GL_UNSIGNED_SHORT, &indexList_[0], vertexPointer());
  list.push_back(cmd);
  return list;
}

#if _IOS_
#pragma mark wire cube
#endif
WireCube::WireCube(GLfloat width, GLfloat height, GLfloat depth, const matsu::vec4 &color)
{
  assert(width > 0 && height > 0 && depth > 0);
  width /= 2;
  height /= 2;
  depth /= 2;


  //top
  //0 1
  //2 3
  //bottom
  //4 5
  //6 7

  //0
  vec3 v0(-width, height, -depth);
  vec3 v1(width, height, -depth);
  vec3 v2(-width, height, depth);
  vec3 v3(width, height, depth);

  vec3 v4(-width, -height, -depth);
  vec3 v5(width, -height, -depth);
  vec3 v6(-width, -height, depth);
  vec3 v7(width, -height, depth);

  //선으로 구성되는거니까 texture, normal은 신경안써도 될듯?
  /*
  vec2 texCoord(0, 0);
  vec3 normal(0, 0, 0);
  matsu::vec4 colorVec(color);

  Vertex3 vertex0(v0, colorVec, texCoord, normal);
  Vertex3 vertex1(v1, colorVec, texCoord, normal);
  Vertex3 vertex2(v2, colorVec, texCoord, normal);
  Vertex3 vertex3(v3, colorVec, texCoord, normal);
  Vertex3 vertex4(v4, colorVec, texCoord, normal);
  Vertex3 vertex5(v5, colorVec, texCoord, normal);
  Vertex3 vertex6(v6, colorVec, texCoord, normal);
  Vertex3 vertex7(v7, colorVec, texCoord, normal);
  */

  matsu::vec4 colorVec(color);

  ColorVertex3 vertex0(v0, colorVec);
  ColorVertex3 vertex1(v1, colorVec);
  ColorVertex3 vertex2(v2, colorVec);
  ColorVertex3 vertex3(v3, colorVec);
  ColorVertex3 vertex4(v4, colorVec);
  ColorVertex3 vertex5(v5, colorVec);
  ColorVertex3 vertex6(v6, colorVec);
  ColorVertex3 vertex7(v7, colorVec);

  vertexList_.push_back(vertex0);
  vertexList_.push_back(vertex1);
  vertexList_.push_back(vertex2);
  vertexList_.push_back(vertex3);
  vertexList_.push_back(vertex4);
  vertexList_.push_back(vertex5);
  vertexList_.push_back(vertex6);
  vertexList_.push_back(vertex7);

  //GL_LINES용 index list
  const GLushort indexList[] = {
    0,1,	1,3,	2,3,	0,2,
    4,5,	5,7,	6,7,	4,6,
    0,4,	1,5,	2,6,	3,7
  };
  for(unsigned int i = 0 ; i < sizeof(indexList) / sizeof(GLushort) ; i++)
  {
    indexList_.push_back(indexList[i]);
  }
}
WireCube::~WireCube()
{
}
const std::vector<DrawCommand> WireCube::getDrawCommand() const
{
  std::vector<DrawCommand> list;
  DrawCommand cmd(GL_LINES, indexList_.size(), GL_UNSIGNED_SHORT, &indexList_[0], vertexPointer());
  list.push_back(cmd);
  return list;
}

#if _IOS_
#pragma mark wire sphere
#endif
WireSphere::WireSphere(GLfloat radius, GLint slices, GLint stacks, const matsu::vec4 &color)
{
  assert(radius > 0 && slices > 0 && stacks > 0);

  //사용될 vertex list 생성
  vector<vec3> vertexList;
  for(int i = 0 ; i < stacks ; i++)
  {
    double yAngle = (M_PI / stacks * i) - M_PI_2;
    float y = static_cast<float>(sin(yAngle));

    for(int j = 0 ; j < slices ; j++)
    {
      double zxAngle = (2.0 * M_PI / slices) * j;
      float x = static_cast<float>(cos(yAngle) * cos(zxAngle));
      float z = static_cast<float>(cos(yAngle) * sin(zxAngle));

      matsu::vec3 pos(x, y, z);
      vertexList.push_back(pos);
    }
  }
  //북극점(맨위)
  matsu::vec3 top(0, 1, 0);
  vertexList.push_back(top);

  //선으로 구성되니까 해당항목은 없어도 별로 티가 안난다
  //vec2 texCoord(0, 0);
  //vec3 normal(0, 0, 0);
  matsu::vec4 colorVec(color);

  //vertex 위치정보+잡탕으로 진짜 vertex list생성
  vector<vec3>::iterator it;
  for(it = vertexList.begin() ; it != vertexList.end() ; it++)
  {
    vec3 &pos = *it;
    pos = pos * radius;
    ColorVertex3 vertex(pos, colorVec);
    vertexList_.push_back(vertex);
  }

  //index list
  for(int i = 0 ; i < stacks ; i++)
  {
    //stack 라인 구성
    int start = i * slices;
    for(int j = 0 ; j < slices ; j++)
    {
      GLushort a = start + j;
      GLushort b = a + 1;
      if(j == slices-1)
        b = start;
      indexList_.push_back(a);
      indexList_.push_back(b);
    }
  }
  for(int i = 0 ; i < slices ; i++)
  {
    //slice 라인 구성
    const GLushort topIndex = vertexList.size()-1;
    for(int j = 0 ; j < stacks ; j++)
    {
      GLushort a = i + (j*slices);
      GLushort b = (a + slices);
      if(b >= slices*stacks)
        b = topIndex;
      indexList_.push_back(a);
      indexList_.push_back(b);
    }
  }
}
WireSphere::~WireSphere()
{
}
const std::vector<DrawCommand> WireSphere::getDrawCommand() const
{
  vector<DrawCommand> list;
  DrawCommand cmd(GL_LINES, indexList_.size(), GL_UNSIGNED_SHORT, &indexList_[0], vertexPointer());
  list.push_back(cmd);
  return list;
}

#if _IOS_
#pragma mark solid sphere
#endif
SolidSphere::SolidSphere(GLfloat radius, GLint slices, GLint stacks, const matsu::vec4 &color)
{
  assert(radius > 0 && slices > 0 && stacks > 0);

  //원을 구성하는 포인트 리스트 + 텍스쳐 좌표 동시 계산
  vector<vec3> posList;
  vector<vec2> texCoordList;
  for(int i = 0 ; i <= stacks ; i++)
  {
    double yAngle = (M_PI / stacks * i) - M_PI_2;
    float y = static_cast<float>(sin(yAngle));
    for(int j = 0 ; j <= slices ; j++)
    {
      float zxAngle = static_cast<float>((2.0 * M_PI / slices) * j);
      float x = static_cast<float>(cos(yAngle) * cos(zxAngle));
      float z = static_cast<float>(cos(yAngle) * sin(zxAngle));

      matsu::vec3 pos(x, y, z);
      posList.push_back(pos);

      //tex 좌표 계산
      float s = 1.0f / (slices) * j;
      float t = 1.0f / (stacks) * i;
      matsu::vec2 texCoord(s, t);
      texCoordList.push_back(texCoord);
    }
  }

  //normal vector 계산
  //normal = 0, 0, 0에서 pos방향 벡터
  vector<vec3> normalList;
  for(std::size_t i = 0 ; i < posList.size() ; i++)
  {
    matsu::vec3 &pos = posList[i];
    vec3 normal = pos.normal();
    normalList.push_back(normal);
  }

  //vertex 구성
  matsu::vec4 colorVec(color);
  for(std::size_t i = 0 ; i < posList.size() ; i++)
  {
    vec3 pos = posList[i] * radius;
    Vertex3 vertex(pos, colorVec, texCoordList[i], normalList[i]);
    vertexList_.push_back(vertex);
  }

  for(int i = 0 ; i < stacks ; i++)
  {
    GLushort start = i * (slices+1);
    for(int j = 0 ; j < slices ; j++)
    {
      //d c
      //b a
      GLushort a = j + start;
      GLushort b = a + 1;
      GLushort c = a + slices + 1;
      GLushort d = c + 1;


      indexList_.push_back(a);
      indexList_.push_back(c);
      indexList_.push_back(b);

      indexList_.push_back(c);
      indexList_.push_back(d);
      indexList_.push_back(b);
    }

  }
}
SolidSphere::~SolidSphere()
{
}
const std::vector<DrawCommand> SolidSphere::getDrawCommand() const
{
  std::vector<DrawCommand> list;
  DrawCommand cmd(GL_TRIANGLES, indexList_.size(), GL_UNSIGNED_SHORT, &indexList_[0], vertexPointer());
  list.push_back(cmd);
  return list;
}

#if _IOS_
#pragma mark wire cone
#endif
WireCone::WireCone(GLfloat base, GLfloat height, GLint slices, GLint stacks, const matsu::vec4 &color)
{
  assert(base > 0 && height > 0 && slices > 0 && stacks > 0);

  //밑면
  vector<vec3> posList;
  for(int j = 0 ; j < stacks ; j++)
  {
    float y = (1.0f / (float)stacks) * j;
    for(int i = 0 ; i < slices ; i++)
    {
      double angleDt = 2.0 * M_PI / slices;
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
  vec4 colorVec(color);
  for(std::size_t i = 0 ; i < posList.size() ; i++)
  {
    vec3 pos = posList[i];
    pos.x *= base;
    pos.y *= height;
    pos.z *= base;
    //Vertex3 v(posList[i], colorVec, texCoord, normal);
    ColorVertex3 v(posList[i], colorVec);
    vertexList_.push_back(v);
  }

  //index for bottom;
  for(int j = 0 ; j < stacks ; j++)
  {
    int startIndex = -1;
    for(int i = 0 ; i < slices ; i++)
    {
      if(i == 0)
        startIndex = j*slices;
      GLushort a = startIndex + i;
      GLushort b = ((a + 1) % slices) + startIndex;
      stackIndexList_.push_back(a);
      stackIndexList_.push_back(b);
    }
  }

  //꼭지점에서 base로 내려오는 직선 긋기
  GLushort topVertexIndex = vertexList_.size()-1;
  for(int i = 0 ; i < slices ; i++)
  {
    sliceIndexList_.push_back(i);
    sliceIndexList_.push_back(topVertexIndex);
  }

}
WireCone::~WireCone()
{
}
const std::vector<DrawCommand> WireCone::getDrawCommand() const
{
  std::vector<DrawCommand> list;
  DrawCommand cmd1(GL_LINES, stackIndexList_.size(), GL_UNSIGNED_SHORT, &stackIndexList_[0], vertexPointer());
  DrawCommand cmd2(GL_LINES, sliceIndexList_.size(), GL_UNSIGNED_SHORT, &sliceIndexList_[0], vertexPointer());
  list.push_back(cmd1);
  list.push_back(cmd2);
  return list;
}

#if _IOS_
#pragma mark solid cone
#endif
SolidCone::SolidCone(GLfloat base, GLfloat height, GLint slices, GLint stacks, const matsu::vec4 &color)
{
  assert(base > 0 && height > 0 && slices > 2 && stacks > 0);
  vec4 colorVec(color);

  //점 목록 찍기. 옆면용 + vertex list 구성
  for(int j = 0 ; j <= stacks ; j++)
  {
    float y = (height / stacks) * j;
    for(int i = 0 ; i <= slices ; i++)
    {
      float angleDt = static_cast<float>(2.0 * M_PI / slices);
      float radius = (1.0f / stacks) * (stacks-j)* base;
      float x = cos(angleDt * i) * radius;
      float z = sin(angleDt * i) * radius;
      vec3 pos(x, y, z);
      vec3 normal = pos.normal();

      float texCoordS = (1.0f / slices) * i;
      float texCoordT = (1.0f / stacks) * j;
      assert(texCoordS >= 0 && texCoordS <= 1 && texCoordT >= 0 && texCoordT <= 1);
      vec2 texCoord(texCoordS, texCoordT);

      Vertex3 v(pos, colorVec, texCoord, normal);
      vertexList_.push_back(v);
    }
  }

  //밑바닥용 vertex pos는 다시 계산한다. normal+tex가 옆면과 달라야되기 떄문에 추가로 필요하다
  vec3 bottomNormal(0, -1, 0);
  const int bottomVertexStartIndex = vertexList_.size();	//bottom vertex 정보가 시작되는 인덱스
  for(int i = 0 ; i < slices ; i++)
  {
    float angleDt = static_cast<float>(2.0 * M_PI / slices);
    float x = cos(angleDt * i) * base;
    float z = sin(angleDt * i) * base;
    vec3 pos(x, 0, z);
    //texcoord 계산
    vec2 texCoord((cos(angleDt * i) + 1) / 2, (sin(angleDt *i) + 1) / 2);

    Vertex3 v(pos, colorVec, texCoord, bottomNormal);
    vertexList_.push_back(v);
  }
  //밑바닥 중심점
  vec3 bottompos(0, 0, 0);
  vec2 bottomCenterTexCoord(0.5, 0.5);
  Vertex3 bottomCenterVertex(bottompos, colorVec, bottomCenterTexCoord, bottomNormal);
  vertexList_.push_back(bottomCenterVertex);

  //index list
  //옆면부터
  for(int i = 0 ; i < stacks ; i++)
  {
    GLushort start = i * (slices+1);
    for(int j = 0 ; j < slices ; j++)
    {
      //x-z평면의 좌표계의 방향은 x-y평면과 증가방향이 반대다
      //d c
      //b a
      GLushort a = start + j;
      GLushort b = a + 1;
      GLushort c = a + (slices + 1);
      GLushort d = c + 1;

      sideIndexList_.push_back(a);
      sideIndexList_.push_back(c);
      sideIndexList_.push_back(d);

      sideIndexList_.push_back(a);
      sideIndexList_.push_back(d);
      sideIndexList_.push_back(b);
    }
  }
  //밑면
  //화면의 밑점은 가장 마지막에 넣었음
  GLushort bottomVertexIndex = vertexList_.size()-1;
  bottomIndexList_.push_back(bottomVertexIndex);
  for(int i = 0 ; i < slices ; i++)
  {
    bottomIndexList_.push_back(i + bottomVertexStartIndex);
  }
  //처음점을 다시 찍어야 원형 완성
  bottomIndexList_.push_back(bottomVertexStartIndex);
}
SolidCone::~SolidCone()
{
}
const std::vector<DrawCommand> SolidCone::getDrawCommand() const
{
  vector<DrawCommand> list;
  DrawCommand bottom(GL_TRIANGLE_FAN, bottomIndexList_.size(), GL_UNSIGNED_SHORT, &bottomIndexList_[0], vertexPointer());
  DrawCommand side(GL_TRIANGLES, sideIndexList_.size(), GL_UNSIGNED_SHORT, &sideIndexList_[0], vertexPointer());
  list.push_back(bottom);
  list.push_back(side);
  return list;
}

//for teapot
class TeapotHelper {
public:
  static Box boundingBox();
  static float scale(GLfloat size);
};
Box TeapotHelper::boundingBox()
{
  vector<SimpleVertex3> vertexlist;

  //set vertex info
  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_VERTEX ; i++)
  {
    //vertex info
    float x = teapotVertices[i*3 + 0];
    float y = teapotVertices[i*3 + 1];
    float z = teapotVertices[i*3 + 2];
    matsu::vec3 pos(x, y, z);

    SimpleVertex3 v(pos);
    vertexlist.push_back(v);
  }
  Box b(vertexlist);
  return b;
}
float TeapotHelper::scale(GLfloat size)
{
  Box b = boundingBox();

  float maxLength = -1;
  if(b.width() > b.height())
    maxLength = b.width();
  else
    maxLength = b.height();

  if(maxLength < b.depth())
    maxLength = b.depth();

  //각 vertex요소를 maxLength로 나누면 크기가 1인 정육면체안에 들어갈수 있다
  //그것에 size를 적용한다
  return 1.0f / maxLength * size;
}

WireTeapot::WireTeapot(GLfloat size, const matsu::vec4 &color)
{
  float scale = TeapotHelper::scale(size);

  //set vertex info
  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_VERTEX ; i++)
  {
    //vertex info
    float x = teapotVertices[i*3 + 0] * scale;
    float y = teapotVertices[i*3 + 1] * scale;
    float z = teapotVertices[i*3 + 2] * scale;
    matsu::vec3 pos(x, y, z);

    ColorVertex3 v(pos, color);
    vertexList_.push_back(v);
  }

  //set index list(for wire)
  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_VERTEX ; i++)
  {
    GLushort index0 = teapotIndices[i*3 + 0];
    GLushort index1 = teapotIndices[i*3 + 1];
    GLushort index2 = teapotIndices[i*3 + 2];

    indexList_.push_back(index0);
    indexList_.push_back(index1);

    indexList_.push_back(index0);
    indexList_.push_back(index2);

    indexList_.push_back(index2);
    indexList_.push_back(index1);
  }
}
WireTeapot::~WireTeapot()
{
}
const std::vector<DrawCommand> WireTeapot::getDrawCommand() const
{
  vector<DrawCommand> list;
  DrawCommand cmd(GL_LINES, indexList_.size(), GL_UNSIGNED_SHORT, &indexList_[0], vertexPointer());
  list.push_back(cmd);
  return list;
}

SolidTeapot::SolidTeapot(GLfloat size, const matsu::vec4 &color)
{
  float scale = TeapotHelper::scale(size);

  //set vertex info
  for(int i = 0 ; i < NUM_TEAPOT_OBJECT_VERTEX ; i++)
  {
    //vertex info
    float x = teapotVertices[i*3 + 0] * scale;
    float y = teapotVertices[i*3 + 1] * scale;
    float z = teapotVertices[i*3 + 2] * scale;
    matsu::vec3 pos(x, y, z);

    //tex coord info
    float s = teapotTexCoords[i*2 + 0];
    float t = teapotTexCoords[i*2 + 1];
    matsu::vec2 texCoord(s, t);

    //normal info
    float normalX = teapotNormals[i*3 + 0];
    float normalY = teapotNormals[i*3 + 1];
    float normalZ = teapotNormals[i*3 + 2];
    matsu::vec3 normal(normalX, normalY, normalZ);

    Vertex3 v(pos, color, texCoord, normal);
    vertexList_.push_back(v);
  }
}
SolidTeapot::~SolidTeapot()
{
}
const std::vector<DrawCommand> SolidTeapot::getDrawCommand() const
{
  vector<DrawCommand> list;
  DrawCommand cmd(GL_TRIANGLES, NUM_TEAPOT_OBJECT_INDEX, GL_UNSIGNED_SHORT, teapotIndices, vertexPointer());
  list.push_back(cmd);
  return list;
}

WireCylinder::WireCylinder(GLfloat radius, GLfloat height, GLint slices, const matsu::vec4 &color)
{
  SR_ASSERT(radius > 0 && height > 0 && slices >= 4);

  //원기둥의 높이(z)는 -height/2~height/2로 height를 구성함
  const float angleDt = static_cast<float>(2.0 * M_PI / slices);
  for(int j = 0 ; j < 2 ; j++)
  {
    for(int i = 0 ; i < slices ; i++)
    {	
      float x = static_cast<float>(cos(angleDt * i) * radius);
      float z = static_cast<float>(sin(angleDt * i) * radius);

      float y = 0;
      if(j == 0)
        y = height / 2;	//윗면
      else
        y = -height / 2;		//아랫면

      matsu::vec3 pos(x, y, z);
      ColorVertex3 v(pos, color);
      vertexList_.push_back(v);
    }
  }
  //밑점, 윗점 따로 vertex list에 넣기
  matsu::vec3 bottomPos(0, -height/2, 0);
  matsu::vec3 topPos(0, height/2, 0);
  ColorVertex3 bottom(bottomPos, color);
  ColorVertex3 top(topPos, color);
  vertexList_.push_back(bottom);
  vertexList_.push_back(top);

  //index list구성
  const int topIndex = vertexList_.size() - 1;
  const int bottomIndex = vertexList_.size() - 2;

  for(int i = 0 ; i < slices ; i++)
  {
    //옆에서 봤을떄의 얻어낸 정보
    //a b
    //c d
    GLushort a = i;
    GLushort c = i + slices;
    GLushort b = (i + 1) % slices;
    GLushort d = (c + 1) % slices + slices;

    //윗면과 아랫면 선으로 연결하기 + 대각선도 넣기
    indexList_.push_back(a);
    indexList_.push_back(c);

    indexList_.push_back(b);
    indexList_.push_back(d);

    indexList_.push_back(a);
    indexList_.push_back(d);

    //윗면의 테두리 그리기	
    indexList_.push_back(a);
    indexList_.push_back(b);

    //아랫면의 테두리 그리기
    indexList_.push_back(c);
    indexList_.push_back(d);

    //아랫면 덮기
    indexList_.push_back(c);
    indexList_.push_back(bottomIndex);

    //윗면 덮기
    indexList_.push_back(a);
    indexList_.push_back(topIndex);
  }
}
WireCylinder::~WireCylinder()
{
}
const std::vector<DrawCommand> WireCylinder::getDrawCommand() const
{
  vector<DrawCommand> list;
  DrawCommand cmd(GL_LINES, indexList_.size(), GL_UNSIGNED_SHORT, &indexList_[0], vertexPointer());
  list.push_back(cmd);
  return list;
}

SolidCylinder::SolidCylinder(GLfloat radius, GLfloat height, GLint slices, const matsu::vec4 &color)
{
  SR_ASSERT(radius > 0 && height > 0 && slices >= 4);

  //원기둥의 높이(z)는 -height/2~height/2로 height를 구성함
  const float angleDt = static_cast<float>(2.0 * M_PI / slices);
  for(int j = 0 ; j < 2 ; j++)
  {
    for(int i = 0 ; i < slices ; i++)
    {	
      float x = static_cast<float>(cos(angleDt * i) * radius);
      float z = static_cast<float>(sin(angleDt * i) * radius);

      float y = 0;
      matsu::vec2 texCoord;
      if(j == 0)
      {
        y = height / 2;	//윗면
        texCoord = matsu::vec2(static_cast<float>(cos(angleDt * i)), 1);
      }	
      else
      {
        y = -height / 2;		//아랫면
        texCoord = matsu::vec2(static_cast<float>(cos(angleDt * i)), 0);
      }

      matsu::vec3 pos(x, y, z);

      //normal 계산
      static matsu::vec3 zero(0, 0, 0);
      matsu::vec3 normal = pos - zero;
      normal.normalize();

      Vertex3 v(pos, color, texCoord, normal);
      vertexList_.push_back(v);
    }
  }
  //밑점, 윗점 따로 vertex list에 넣기
  //밑점
  {
    matsu::vec3 bottomPos(0, -height/2, 0);
    matsu::vec2 texCoord(0, 0);
    matsu::vec3 normal(0, -1, 0);
    Vertex3 bottom(bottomPos, color, texCoord, normal);
    vertexList_.push_back(bottom);
  }

  //윗점
  {
    matsu::vec3 topPos(0, height/2, 0);
    matsu::vec2 texCoord(0, 0);
    matsu::vec3 normal(0, 1, 0);
    Vertex3 top(topPos, color, texCoord, normal);
    vertexList_.push_back(top);
  }

  //index list구성
  const int topIndex = vertexList_.size() - 1;
  const int bottomIndex = vertexList_.size() - 2;

  topIndexList_.push_back(topIndex);
  bottomIndexList_.push_back(bottomIndex);

  for(int i = 0 ; i < slices ; i++)
  {
    //옆에서 봤을떄의 얻어낸 정보
    //a b
    //c d
    GLushort a = i;
    GLushort c = i + slices;
    GLushort b = (i + 1) % slices;
    GLushort d = (c + 1) % slices + slices;

    //옆면 삼각형 인덱스 생성
    sideIndexList_.push_back(a);
    sideIndexList_.push_back(c);
    sideIndexList_.push_back(d);

    sideIndexList_.push_back(a);
    sideIndexList_.push_back(d);
    sideIndexList_.push_back(b);
  }

  for(int i = 0 ; i <= slices ; i++)
  {
    //옆에서 봤을떄의 얻어낸 정보
    //a b
    //c d
    GLushort a = (i) % slices;
    GLushort c = a + slices;

    //아랫면 덮기
    bottomIndexList_.push_back(c);
  }

  for(int i = slices-1 ; i >= -1 ; i--)
  {
    //옆에서 봤을떄의 얻어낸 정보
    //a b
    //c d
    GLushort a = (i) % slices;
    if(i == -1)
      a = slices-1;

    //윗면 덮기, (인덱스 넣는 방향 반대여야 cull통과한다)
    topIndexList_.push_back(a);
  }
}
SolidCylinder::~SolidCylinder()
{
}
const std::vector<DrawCommand> SolidCylinder::getDrawCommand() const
{
  vector<DrawCommand> list;
  DrawCommand topCmd(GL_TRIANGLE_FAN, topIndexList_.size(), GL_UNSIGNED_SHORT, &topIndexList_[0], vertexPointer());
  DrawCommand bottomCmd(GL_TRIANGLE_FAN, bottomIndexList_.size(), GL_UNSIGNED_SHORT, &bottomIndexList_[0], vertexPointer());
  DrawCommand sideCmd(GL_TRIANGLE_STRIP, sideIndexList_.size(), GL_UNSIGNED_SHORT, &sideIndexList_[0], vertexPointer());
  list.push_back(topCmd);
  list.push_back(bottomCmd);
  list.push_back(sideCmd);
  return list;
}

WireAxis::WireAxis(GLfloat size, const matsu::vec4 &xColor, const matsu::vec4 &yColor, const matsu::vec4 &zColor)
{
  init(size, xColor, yColor, zColor);
}
WireAxis::WireAxis(GLfloat size)
{
  static const color4 red = Color::red();
  static const color4 green = Color::green();
  static const color4 blue = Color::blue();
  init(size, red, green, blue);
}
WireAxis::~WireAxis()
{
}
const std::vector<DrawCommand> WireAxis::getDrawCommand() const
{
  vector<DrawCommand> list;
  DrawCommand cmd(GL_LINES, indexList_.size(), GL_UNSIGNED_SHORT, &indexList_[0], vertexPointer());
  list.push_back(cmd);
  return list;
}
void WireAxis::init(float size, const matsu::vec4 &xColor, const matsu::vec4 &yColor, const matsu::vec4 &zColor)
{
  SR_ASSERT(size > 0);
  //vertex list 생성
  matsu::vec3 xPos(size, 0, 0);
  matsu::vec3 yPos(0, size, 0);
  matsu::vec3 zPos(0, 0, size);
  matsu::vec3 zero(0, 0, 0);

  //x axis
  ColorVertex3 vx0(zero, xColor);
  ColorVertex3 vx1(xPos, xColor);
  vertexList_.push_back(vx0);
  vertexList_.push_back(vx1);
  //y axis
  ColorVertex3 vy0(zero, yColor);
  ColorVertex3 vy1(yPos, yColor);
  vertexList_.push_back(vy0);
  vertexList_.push_back(vy1);
  //z axis
  ColorVertex3 vz0(zero, zColor);
  ColorVertex3 vz1(zPos, zColor);
  vertexList_.push_back(vz0);
  vertexList_.push_back(vz1);

  //create index list
  for(GLushort i = 0 ; i < 6 ; i++)
    indexList_.push_back(i);
}

#endif
}

