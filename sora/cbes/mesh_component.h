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
#ifndef SORA_MESH_COMPONENT_H_
#define SORA_MESH_COMPONENT_H_

#include "component.h"

#include "renderer/primitive_model.h"
#include "renderer/obj_model.h"

namespace sora {;
class Renderer;
class PrimitiveModel;
class ObjModel;

class MeshComponent : public Component {
public:
  enum {
    kMeshNone,
    kMeshObj,
    kMeshPrimitive,
  };
public:
  SR_CHILD_CLASS_2("mesh");
  MeshComponent(Entity *entity);
  ~MeshComponent();

public:
  void SetMesh(const PrimitiveModel &primitive);
  void SetMesh(const ObjModel &obj);

  int mesh_type() const { return mesh_type_; }
  const ObjModel &obj_model() const { return obj_model_; }
  const PrimitiveModel &primitive_model() const { return primitive_model_; }

private:
  //model을 타입열로 간단히 분기하기
  //나중에 구조가 복잡해지면 그떄 분리같은거 생각하자
  int mesh_type_;
  ObjModel obj_model_;
  PrimitiveModel primitive_model_; 
};
}

#endif  // SORA_MESH_COMPONENT_H_