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
#ifndef SORA_MATRIX_STACK_H_
#define SORA_MATRIX_STACK_H_

#include <glm/glm.hpp>
#include "vector.h"

namespace sora {;
class SR_DLL MatrixStack {
public:
  MatrixStack();
  ~MatrixStack();

  void Push();
  void Pop();
  const glm::mat4 &Top() const;
  int Size() const;
  // remove all data and push identity matrix
  void Clear();

  void Load(const glm::mat4 &m);
  void Mult(const glm::mat4 &m);
  void SetIdentity();
  
  void Scale(float x, float y, float z);
  void Scale(float t);
  
  void Translate(float x, float y, float z);
  void Translate(const glm::vec3 &v);
  void Translate(const vec3f_t &v);
  void Translate(const Vec3f &v);

  void RotateX(float deg);
  void RotateY(float deg);
  void RotateZ(float deg);

  void Rotate(float deg, float x, float y, float z);
  void Rotate(float deg, const glm::vec3 &v);
  void Rotate(float deg, const vec3f_t &v);
  void Rotate(float deg, const Vec3f &v);

private:
  std::vector<glm::mat4> matrix_stack_;
};
}

#endif  // SORA_MATRIX_STACK_H_