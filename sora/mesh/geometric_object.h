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

#include "core/vertex.h"
#include "renderer/globals.h"
#include "mesh/parametric_surface.h"
#include "mesh/parametric_equations.h"

namespace sora {;

//기본도형은 normal까지만 지원하낟. 그 이상의 정보가 필요하면
//버텍스 목록에 추가 가공을 다른방식으로 집어넣자
struct DrawCmdData {
  DrawType draw_mode;
  std::vector<Vertex> vertex_list;
  std::vector<unsigned short> index_list;
};

class GeometricObject {
public:
  void PointCube(float width, float height, float depth);
  void WireCube(float width, float height, float depth);
  void SolidCube(float width, float height, float depth);

  void PointTeapot( float size );
  void WireTeapot( float size );
  void SolidTeapot( float size );

  void PointShpere(float radius, int slices, int stacks);
  void WireShpere(float radius, int slices, int stacks);
  void SolidSphere(float radius, int slices, int stacks);

  void PointCylinder(float baseRadius, float topRadius, float height, int slices, int stacks);
  void WireCylinder(float baseRadius, float topRadius, float height, int slices, int stacks);
  void SolidCylinder(float baseRadius, float topRadius, float height, int slices, int stacks);

  void WireAxis(float size);

  void WirePlane(float half_size, float grid_size);
  void SolidPlane(float half_size);

  void Clear() { cmd_list_.clear(); }

  std::vector<DrawCmdData>::iterator Begin() { return cmd_list_.begin(); }
  std::vector<DrawCmdData>::iterator End() { return cmd_list_.end(); }

private:
  std::vector<DrawCmdData> cmd_list_;  
};

// ParametricSurface 를 기반으로 구성. tangent까지 지원하기 위해서 일단 분리함
template<typename VertexType>
class ParametricObject {
public:
  ParametricObject() : draw_mode_(kDrawPoints) {}
  void Clear() { vert_list_.clear(); index_list_.clear(); draw_mode_ = kDrawPoints; }
  const std::vector<VertexType> &vertex_list() { return vert_list_; }
  const std::vector<unsigned short> &index_list() const { return index_list_; }
  DrawType draw_mode() const { return draw_mode_; }

  void WireCone(float height, float radius) {
    Cone mesh(height, radius);
    BuildWireMesh(mesh);
  }
  void SolidCone(float height, float radius) {
    Cone mesh(height, radius);
    BuildSolidMesh(mesh);
  }

  void WireSphere(float radius) {
    Sphere mesh(radius);
    BuildWireMesh(mesh);
  }
  void SolidSphere(float radius) {
    Sphere mesh(radius);
    BuildSolidMesh(mesh);
  }

  void WireTorus(float majorRadius, float minorRadius) {
    Torus mesh(majorRadius, minorRadius);
    BuildWireMesh(mesh);
  }
  void SolidTorus(float majorRadius, float minorRadius) {
    Torus mesh(majorRadius, minorRadius);
    BuildSolidMesh(mesh);
  }

  void WireTrefoilKnot(float scale) {
    TrefoilKnot mesh(scale);
    BuildWireMesh(mesh);
  }
  void SolidTrefoilKnot(float scale) {
    TrefoilKnot mesh(scale);
    BuildSolidMesh(mesh);
  }

  void WireMobiusStrip(float scale) {
    MobiusStrip mesh(scale);
    BuildWireMesh(mesh);
  }
  void SolidMobiusStrip(float scale) {
    MobiusStrip mesh(scale);
    BuildSolidMesh(mesh);
  }

  void WireKleinBottle(float scale) {
    KleinBottle mesh(scale);
    BuildWireMesh(mesh);
  }
  void SolidKleinBottle(float scale) {
    KleinBottle mesh(scale);
    BuildSolidMesh(mesh);
  }

private:
  template<typename T>
  void BuildWireMesh(const T &mesh) {
    Clear();
    mesh.GenerateVertices(vert_list_);
    mesh.GenerateLineIndices(index_list_);
    draw_mode_ = kDrawLines;
  }
  template<typename T>
  void BuildSolidMesh(const T &mesh) {
    Clear();
    mesh.GenerateVertices(vert_list_);
    mesh.GenerateTriangleIndices(index_list_);
    draw_mode_ = kDrawTriangles;
  }
private:
  DrawType draw_mode_;
  std::vector<VertexType> vert_list_;
  std::vector<unsigned short> index_list_;
};
}

#endif // SORA_GEOMETRIC_OBJECT_H_