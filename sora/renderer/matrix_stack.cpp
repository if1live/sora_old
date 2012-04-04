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

#include "matrix_stack.h"
#if SR_USE_PCH == 0
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#endif

namespace sora {;
struct MatrixStackImpl {
  std::vector<glm::mat4> matrix_stack;
};

MatrixStack::MatrixStack() : impl(new MatrixStackImpl()){
  Clear();
}
MatrixStack::~MatrixStack() {
  delete(impl);
}
const glm::mat4 &MatrixStack::Top() const {
  SR_ASSERT(impl->matrix_stack.empty() == false);
  return impl->matrix_stack.back();
}
void MatrixStack::Push() {
  const glm::mat4 &top = Top();
  impl->matrix_stack.push_back(top);
}
void MatrixStack::Pop() {
  if(impl->matrix_stack.size() > 0) {
    impl->matrix_stack.pop_back();
  }
  //스택의 요소가 전부 빠지면 understack으로 하는 대신
  //기본행렬을 넣어서 방어하자
  if(impl->matrix_stack.size() == 0) {
    Clear();
  }
}
int MatrixStack::Size() const {
  return impl->matrix_stack.size();
}
// remove all data and push identity matrix
void MatrixStack::Clear() {
  impl->matrix_stack.clear();
  glm::mat4 m = glm::mat4(1.0f);
  impl->matrix_stack.push_back(m);
}

void MatrixStack::Load(const glm::mat4 &m) {
  glm::mat4 &top = impl->matrix_stack.back();
  top = m;
}
void MatrixStack::Mult(const glm::mat4 &m) {
  glm::mat4 &top = impl->matrix_stack.back();
  top *= m;
}
void MatrixStack::SetIdentity() {
  glm::mat4 m = glm::mat4(1.0f);
  Load(m);
}
  
void MatrixStack::Scale(float x, float y, float z) {
  glm::vec3 v(x, y, z); 
  glm::mat4 &top = impl->matrix_stack.back();
  glm::mat4 m = glm::scale(top, v);
  top = m;
}
void MatrixStack::Scale(float t) {
  Scale(t, t, t);
}
  
void MatrixStack::Translate(float x, float y, float z) {
  glm::vec3 v(x, y, z);
  Translate(v);
}
void MatrixStack::Translate(const glm::vec3 &v) {
  glm::mat4 &top = impl->matrix_stack.back();
  glm::mat4 m = glm::translate(top, v);
  top = m;
}

void MatrixStack::RotateX(float deg) {
  Rotate(deg, glm::vec3(1.0f, 0.0f, 0.0f));
}
void MatrixStack::RotateY(float deg) {
  Rotate(deg, glm::vec3(0.0f, 1.0f, 0.0f));
}
void MatrixStack::RotateZ(float deg) {
  Rotate(deg, glm::vec3(0.0f, 0.0f, 1.0f));
}

void MatrixStack::Rotate(float deg, float x, float y, float z) {
  glm::vec3 v(x, y, z);
  Rotate(deg, v);
}
void MatrixStack::Rotate(float deg, const glm::vec3 &v) {
  glm::mat4 &top = impl->matrix_stack.back();
  glm::mat4 m = glm::rotate(top, deg, v);
  top = m;
}
}
