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
#include "mesh_component.h"
#include "template_lib.h"
#include "entity.h"

#include "obj_model.h"
#include "primitive_model.h"
#include "renderer.h"
#include "filesystem.h"

namespace sora {;
enum {
  kMeshNone,
  kMeshObj,
  kMeshPrimitive,
};
struct MeshComponentImpl {
  MeshComponentImpl() : mesh_type(kMeshNone) {}
  //model을 타입열로 간단히 분기하기
  //나중에 구조가 복잡해지면 그떄 분리같은거 생각하자
  int mesh_type;
  ObjModel obj_model;
  PrimitiveModel primitive_model;
};

MeshComponent::MeshComponent(Entity *entity)
: Component(entity, ClassType()), impl(new MeshComponentImpl()) {
}
MeshComponent::~MeshComponent() {
  SafeDelete(impl);
}

void MeshComponent::Draw(Renderer *renderer) {
  switch(impl->mesh_type) {
  case kMeshObj:
    renderer->ApplyMatrix3D(entity()->world_mat());
    renderer->DrawObj(impl->obj_model);
    break;
  case kMeshPrimitive:
    renderer->ApplyMatrix3D(entity()->world_mat());
    renderer->DrawPrimitiveModel(impl->primitive_model);
    break;
  default:
    break;
  }
}

void MeshComponent::SetMesh(const PrimitiveModel &primitive) {
  impl->mesh_type = kMeshPrimitive;
  impl->primitive_model = primitive;
}
void MeshComponent::SetMesh(const ObjModel &obj) {
  impl->mesh_type = kMeshObj;
  impl->obj_model = obj;
}
}