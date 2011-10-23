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
#ifndef RENDERING_LYNETTE_MODEL_H_
#define RENDERING_LYNETTE_MODEL_H_

#include <vector>
#include "lynette/lynette_enum.h"
#include "matsu/matsu_enum.h"

namespace lynette {;		
///@brief sora내부에서 사용되는 모델 클래스이다. 외부 모델은 이 형식에 맞춰서 변환된 후에
///그려질수 있다. glut에 있던 구를 그리거나 큐브를 그리는 함수의 경우도 이 클래스의 인스턴스를
///생성한 후에 그린다. 기능상은 엔진에서 쓰이니까 이름을 Model로 하는것이 좋을듯하지만
///왠지모르게 이름 충돌의 가능성이 느껴져서 sora를 붙였다. 
///vertex를 저장할때의 각각의 정보의 크기는...
///pos : float * 3
///color : float * 4
///texcoord : float * 2
///normal : float * 3
///으로 일단 정해져있다. 		
class Model {
public:
  Model(unsigned int drawMode, unsigned int useFlag = 0);
  ~Model();

  ///@brief vertex정보를 채우기
  void addVertexData(float a, float b);
  void addVertexData(float a, float b, float c);
  void addVertexData(float a, float b, float c, float d);
  void addVertexData(const matsu::vec2 &v);
  void addVertexData(const matsu::vec3 &v);
  void addVertexData(const matsu::vec4 &v);
  void setVertexData(const std::vector<float> &data);

  ///@brief index 정보 채우기
  void addIndexData(unsigned short a);
  void setIndexData(const std::vector<unsigned short> &data);

  const float* getVertexData() const;
  const unsigned short* getIndexData() const;
  int getStride() const;
  int getIndexCount() const;
  int getVertexCount() const;

  const float* getPosPtr() const;
  const float* getColorPtr() const;
  const float* getTexCoordPtr() const;
  const float* getNormalPtr() const;

  bool isUseColor() const;
  bool isUseTexCoord() const;
  bool isUseNormal() const;
  unsigned int getDrawMode() const;
  bool isIndexDataExist() const;

private:
  bool useColor_;
  bool useTexCoord_;
  bool useNormal_;

  ///@brief 어떤 형태로 그릴것인지를 결정
  unsigned int drawMode_;

  ///@brief vertex를 구성하는 정보를 저장하는 배열이다. 모든 데이터는 통째로 여기에 들어간다
  ///Vertex3같은 클래스를 사용하지 않는 이유는 필요한 정보만 저장해서 메모리를 아끼기 위해서이닫
  ///배열에 저장될때의 우선순의는 pos > color > texCoord > normal이다
  std::vector<float> vertexdata_;

  ///@brief vertex의 index정보를 가지고 있는 배열이다. 
  ///비어있으면 인덱스를 이용해서 그리지 않는것으로 갅주한다
  std::vector<unsigned short> indexdata_;
};
}

#endif  // RENDERING_LYNETTE_MODEL_H_