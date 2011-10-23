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
#include "precompile.h"
#include "lynette/model.h"
#include "lynette/lynette_enum.h"
#include "matsu/vector.h"

using matsu::vec2;
using matsu::vec3;
using matsu::vec4;

namespace lynette {;
Model::Model(unsigned int drawMode, unsigned int useFlag)
  : useColor_(false), useTexCoord_(false), useNormal_(false), drawMode_(drawMode) {
  if((useFlag & LYNETTE_USE_COLOR) == LYNETTE_USE_COLOR) {
    useColor_ = true;
  }				
  if((useFlag & LYNETTE_USE_TEXCOORD) == LYNETTE_USE_TEXCOORD) {
    useTexCoord_ = true;
  }
  if((useFlag & LYNETTE_USE_NORMAL) == LYNETTE_USE_NORMAL) {
    useNormal_ = true;
  }
}
Model::~Model() {
}
const float* Model::getVertexData() const {
  SR_ASSERT(vertexdata_.size() > 0 && "vertex data is empty!!");
  return &vertexdata_[0];
}
const unsigned short* Model::getIndexData() const {
  return &indexdata_[0];
}
const float* Model::getPosPtr() const {
  return getVertexData();
}
const float* Model::getColorPtr() const {
  SR_ASSERT(isUseColor());
  return getPosPtr() + 3;
}
const float* Model::getTexCoordPtr() const {
  SR_ASSERT(isUseTexCoord());
  float* ptr = const_cast<float*>(getPosPtr());
  ptr += 3;	//pos end
  if(isUseColor()) {
    ptr += 4;
  }
  return ptr;
}
const float* Model::getNormalPtr() const {
  SR_ASSERT(isUseNormal());
  float* ptr = const_cast<float*>(getPosPtr());
  ptr += 3;	//pos end
  if(isUseColor()) {
    ptr += 4;
  }
  if(isUseTexCoord()) {
    ptr += 2;
  }
  return ptr;
}
int Model::getStride() const {
  ///pos용 3개는 항상 기본
  int stride = 3;
  if(isUseColor() == true) {
    stride += 4;
  }
  if(isUseTexCoord() == true) {
    stride += 2;
  }
  if(isUseNormal() == true) {
    stride += 3;
  }
  return stride * sizeof(float);
}
int Model::getIndexCount() const {
  return indexdata_.size();
}
int Model::getVertexCount() const {
  unsigned int size = vertexdata_.size() * sizeof(float);
  unsigned int stride = getStride();
  return size / stride;
}

bool Model::isUseTexCoord() const {
  return useTexCoord_;
}
bool Model::isUseColor() const {
  return useColor_;
}
bool Model::isUseNormal() const {
  return useNormal_;
}
unsigned int Model::getDrawMode() const {
  return drawMode_;
}
bool Model::isIndexDataExist() const {
  if(indexdata_.size() == 0) {
    return false;
  } else {
    return true;
  }
}
void Model::addVertexData(float a, float b) {
  vertexdata_.push_back(a);
  vertexdata_.push_back(b);
}
void Model::addVertexData(float a, float b, float c) {
  vertexdata_.push_back(a);
  vertexdata_.push_back(b);
  vertexdata_.push_back(c);
}
void Model::addVertexData(float a, float b, float c, float d) {
  vertexdata_.push_back(a);
  vertexdata_.push_back(b);
  vertexdata_.push_back(c);
  vertexdata_.push_back(d);
}
void Model::addVertexData(const matsu::vec2 &v) {
  vertexdata_.push_back(v.x());
  vertexdata_.push_back(v.y());
}
void Model::addVertexData(const matsu::vec3 &v) {
  vertexdata_.push_back(v.x());
  vertexdata_.push_back(v.y());
  vertexdata_.push_back(v.z());
}
void Model::addVertexData(const matsu::vec4 &v) {
  vertexdata_.push_back(v.x());
  vertexdata_.push_back(v.y());
  vertexdata_.push_back(v.z());
  vertexdata_.push_back(v.w());
}

void Model::setVertexData(const std::vector<float> &data) {
  vertexdata_ = data;
}

void Model::addIndexData(unsigned short a) {
  indexdata_.push_back(a);
}
void Model::setIndexData(const std::vector<unsigned short> &data) {
  indexdata_ = data;
}
}