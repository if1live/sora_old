/*
* Copyright (c) 2002-2008 LWJGL Project
* All rights reserved.
* modified by if1live 2012
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
#ifndef SORA_GEOMETRIC_OBJECT_H_
#define SORA_GEOMETRIC_OBJECT_H_

#include "vertex.h"
#include "globals.h"
#include "parametric_surface.h"
#include "parametric_equations.h"

namespace sora {;

class PrimitiveMeshHelper;
class BasicPrimitiveMeshHelper;

//내장 도형을 그리는 최상위 인터페이스
//tangent vertex / vertex를 동시에 지원하기 위해서 적절히 템플릿으로 처리함
template<typename T>
class GeometricObject {
public:
  typename std::vector< DrawCmdData<T> >::iterator Begin() { return cmd_list_.begin(); }
  typename std::vector< DrawCmdData<T> >::iterator End() { return cmd_list_.end(); }
  const std::vector< DrawCmdData<T> > &cmd_list() const { return cmd_list_; }

  void PointCube(float width, float height, float depth) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.PointCube(width, height, depth);
  }
  void WireCube(float width, float height, float depth) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.WireCube(width, height, depth);
  }
  void SolidCube(float width, float height, float depth) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.SolidCube(width, height, depth);
  }

  void PointTeapot( float size ) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.PointTeapot(size);
  }
  void WireTeapot( float size ) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.WireTeapot(size);
  }
  void SolidTeapot( float size ) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.SolidTeapot(size);
  }

  void PointSphere(float radius, int slices, int stacks) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.PointShpere(radius, slices, stacks);
  }
  void WireSphere(float radius, int slices, int stacks) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.WireShpere(radius, slices, stacks);
  }

  void SolidSphere(float radius, int slices, int stacks) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.SolidSphere(radius, slices, stacks);
  }

  void PointCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.PointCylinder(baseRadius, topRadius, height, slices, stacks);
  }
  void WireCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.WireCylinder(baseRadius, topRadius, height, slices, stacks);
  }
  void SolidCylinder(float baseRadius, float topRadius, float height, int slices, int stacks) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.SolidCylinder(baseRadius, topRadius, height, slices, stacks);
  }

  void WireAxis(float size) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.WireAxis(size);
  }

  void WirePlane(float half_size, float grid_size) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.WirePlane(half_size, grid_size);
  }
  void SolidPlane(float half_size) {
    Clear();
    PrimitiveMeshHelper helper(&cmd_list_);
    helper.SolidPlane(half_size);
  }

  //좀 복잡한 형태
  void WireCone(float height, float radius) {
    Cone mesh(height, radius);
    BuildWireParametricMesh(mesh);
  }
  void SolidCone(float height, float radius) {
    Cone mesh(height, radius);
    BuildSolidParametricMesh(mesh);
  }

  void WireSphere(float radius) {
    Sphere mesh(radius);
    BuildWireParametricMesh(mesh);
  }
  void SolidSphere(float radius) {
    Sphere mesh(radius);
    BuildSolidParametricMesh(mesh);
  }

  void WireTorus(float majorRadius, float minorRadius) {
    Torus mesh(majorRadius, minorRadius);
    BuildWireParametricMesh(mesh);
  }
  void SolidTorus(float majorRadius, float minorRadius) {
    Torus mesh(majorRadius, minorRadius);
    BuildSolidParametricMesh(mesh);
  }

  void WireTrefoilKnot(float scale) {
    TrefoilKnot mesh(scale);
    BuildWireParametricMesh(mesh);
  }
  void SolidTrefoilKnot(float scale) {
    TrefoilKnot mesh(scale);
    BuildSolidParametricMesh(mesh);
  }

  void WireMobiusStrip(float scale) {
    MobiusStrip mesh(scale);
    BuildWireParametricMesh(mesh);
  }
  void SolidMobiusStrip(float scale) {
    MobiusStrip mesh(scale);
    BuildSolidParametricMesh(mesh);
  }

  void WireKleinBottle(float scale) {
    KleinBottle mesh(scale);
    BuildWireParametricMesh(mesh);
  }
  void SolidKleinBottle(float scale) {
    KleinBottle mesh(scale);
    BuildSolidParametricMesh(mesh);
  }

  void Clear() { cmd_list_.clear(); }

private:
  template<typename T>
  void BuildWireParametricMesh(const T &mesh) {
    Clear();
    cmd_list_.resize(1);
    auto &cmd = cmd_list_[0];
    cmd.draw_mode = kDrawLines;
    mesh.GenerateVertices(cmd.vertex_list);
    mesh.GenerateLineIndices(cmd.index_list);
  }
  template<typename T>
  void BuildSolidParametricMesh(const T &mesh) {
    Clear();
    cmd_list_.resize(1);
    auto &cmd = cmd_list_[0];
    cmd.draw_mode = kDrawTriangles;
    mesh.GenerateVertices(cmd.vertex_list);
    mesh.GenerateTriangleIndices(cmd.index_list);
  }  

private:
  std::vector< DrawCmdData<T> > cmd_list_;
};

//vertex기반의 도형 생성기. 이것에 다른것을 적절히 달아서 쓰자
class PrimitiveMeshHelper {
public:
  PrimitiveMeshHelper(std::vector< DrawCmdData<Vertex> > *cmd_list) : cmd_list_(cmd_list) {}
  void PointCube(float width, float height, float depth);
  void WireCube(float width, float height, float depth);
  void SolidCube(float width, float height, float depth);

  void PointTeapot( float size );
  void WireTeapot( float size );
  void SolidTeapot( float size );

  void PointSphere(float radius, int slices, int stacks);
  void WireSphere(float radius, int slices, int stacks);
  void SolidSphere(float radius, int slices, int stacks);

  void PointCylinder(float baseRadius, float topRadius, float height, int slices, int stacks);
  void WireCylinder(float baseRadius, float topRadius, float height, int slices, int stacks);
  void SolidCylinder(float baseRadius, float topRadius, float height, int slices, int stacks);

  void WireAxis(float size);

  void WirePlane(float half_size, float grid_size);
  void SolidPlane(float half_size); 

private:
  std::vector< DrawCmdData<Vertex> > *cmd_list_;  
};

class BasicPrimitiveMeshHelper {
public:
  BasicPrimitiveMeshHelper(std::vector< DrawCmdData<glm::vec3> > *cmd_list) : cmd_list_(cmd_list) {}
  void WireSphere(float radius, int slices, int stacks);
private:
  std::vector< DrawCmdData<glm::vec3> > *cmd_list_;  
};



template<>
void GeometricObject<glm::vec3>::WireSphere(float radius, int slices, int stacks) {
  Clear();
  BasicPrimitiveMeshHelper helper(&cmd_list_);
  helper.WireSphere(radius, slices, stacks);
}
} // namespace sora
#endif // SORA_GEOMETRIC_OBJECT_H_