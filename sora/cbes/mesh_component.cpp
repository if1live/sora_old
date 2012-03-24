﻿/*  Copyright (C) 2011-2012 by if1live */
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
#include "core/template_lib.h"
#include "entity.h"

#include "renderer/obj_model.h"
#include "renderer/primitive_model.h"
#include "renderer/renderer.h"
#include "sys/filesystem.h"

namespace sora {;

MeshComponent::MeshComponent(Entity *entity)
: Component(entity, ClassType()),
mesh_type_(kMeshNone) {
}
MeshComponent::~MeshComponent() {
}

void MeshComponent::SetMesh(const PrimitiveModel &primitive) {
  mesh_type_ = kMeshPrimitive;
  primitive_model_ = primitive;
}
void MeshComponent::SetMesh(const ObjModel &obj) {
  mesh_type_ = kMeshObj;
  obj_model_ = obj;
}
}