/*
 * freeglut_std.h
 *
 * The GLUT-compatible part of the freeglut library include file
 *
 * Copyright (c) 1999-2000 Pawel W. Olszta. All Rights Reserved.
 * Written by Pawel W. Olszta, <olszta@sourceforge.net>
 * Creation date: Thu Dec 2 1999
 * modified by if1live (2012)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef SORA_GEOMETRIC_OBJECT_H_
#define SORA_GEOMETRIC_OBJECT_H_

#include "core/vertex.h"
#include "renderer/globals.h"

namespace sora {;
/*
 * Font stuff, see freeglut_font.c
 */
/*
void glutStrokeCharacter( void* font, int character );
int glutStrokeWidth( void* font, int character );
int glutBitmapLength( void* font, const unsigned char* string );
int glutStrokeLength( void* font, const unsigned char* string );
float glutStrokeHeight( void* font );
void glutStrokeString( void* font, const unsigned char *string );
*/
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
  void Clear() { cmd_list_.clear(); }

  std::vector<DrawCmdData>::iterator Begin() { return cmd_list_.begin(); }
  std::vector<DrawCmdData>::iterator End() { return cmd_list_.end(); }

private:
  std::vector<DrawCmdData> cmd_list_;  
};
/*
 * Geometry functions, see freeglut_geometry.c
 */
/*
void glutWireCube( float size );
void glutSolidCube( float size );
void glutWireSphere( float radius, int slices, int stacks );
void glutSolidSphere( float radius, int slices, int stacks );
void glutWireCone( float base, float height, int slices, int stacks );
void glutSolidCone( float base, float height, int slices, int stacks );

void glutWireTorus( float innerRadius, float outerRadius, int sides, int rings );
void glutSolidTorus( float innerRadius, float outerRadius, int sides, int rings );
void glutWireDodecahedron();
void glutSolidDodecahedron();
void glutWireOctahedron();
void glutSolidOctahedron();
void glutWireTetrahedron();
void glutSolidTetrahedron();
void glutWireIcosahedron();
void glutSolidIcosahedron();

void glutWireRhombicDodecahedron( );
void glutSolidRhombicDodecahedron();
void glutWireSierpinskiSponge ( int num_levels, float offset[3], float scale );
void glutSolidSierpinskiSponge ( int num_levels, float offset[3], float scale );
void glutWireCylinder( float radius, float height, int slices, int stacks);
void glutSolidCylinder( float radius, float height, int slices, int stacks);
*/
}

#endif // SORA_GEOMETRIC_OBJECT_H_