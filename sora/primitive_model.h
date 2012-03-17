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
#ifndef SORA_PRIMITIVE_MODEL_H_
#define SORA_PRIMITIVE_MODEL_H_

namespace sora {;
#if 0
class PrimitiveModel : public Model {
public:
  PrimitiveModel() {}
  virtual ~PrimitiveModel() {}
  ModelType type() const;
protected:
  virtual VertexPointer vertexPointer() const = 0;
};

//wire frame의 경우는 색+좌표만 있어도 된다
class WirePrimitiveModel : public PrimitiveModel {
public:
  WirePrimitiveModel() {}
  virtual ~WirePrimitiveModel() {}
protected:
  VertexPointer vertexPointer() const;
  std::vector<ColorVertex3> vertexList_;
};

//solid model의 경우 색+좌표+텍스쳐+노멀이 필요
class SolidPrimitiveModel : public PrimitiveModel {
public:
  SolidPrimitiveModel() {}
  virtual ~SolidPrimitiveModel() {}
protected:
  VertexPointer vertexPointer() const;
  std::vector<Vertex3> vertexList_;
};

class WireCube : public WirePrimitiveModel {
public:
  WireCube(GLfloat width, GLfloat height, GLfloat depth, const matsu::vec4 &color);
  virtual ~WireCube();
  const std::vector<DrawCommand> getDrawCommand() const;
private:
  std::vector<GLushort> indexList_;
};
class SolidCube : public SolidPrimitiveModel {
public:
  SolidCube(GLfloat width, GLfloat height, GLfloat depth, const matsu::vec4 &color);
  virtual ~SolidCube();
  const std::vector<DrawCommand> getDrawCommand() const;
private:
  std::vector<GLushort> indexList_;
};

class WireSphere : public WirePrimitiveModel {
public:
  WireSphere(GLfloat radius, GLint slices, GLint stacks, const matsu::vec4 &color);
  virtual ~WireSphere();
  const std::vector<DrawCommand> getDrawCommand() const;
private:
  std::vector<GLushort> indexList_;

};
class SolidSphere : public SolidPrimitiveModel {
public:
  SolidSphere(GLfloat radius, GLint slices, GLint stacks, const matsu::vec4 &color);
  virtual ~SolidSphere();
  const std::vector<DrawCommand> getDrawCommand() const;
private:
  std::vector<GLushort> indexList_;
};

class WireCone : public WirePrimitiveModel {
public:
  WireCone(GLfloat base, GLfloat height, GLint slices, GLint stacks, const matsu::vec4 &color);
  virtual ~WireCone();
  const std::vector<DrawCommand> getDrawCommand() const;
private:
  std::vector<GLushort> stackIndexList_;
  std::vector<GLushort> sliceIndexList_;
};
class SolidCone : public SolidPrimitiveModel {
public:
  SolidCone(GLfloat base, GLfloat height, GLint slices, GLint stacks, const matsu::vec4 &color);
  virtual ~SolidCone();
  const std::vector<DrawCommand> getDrawCommand() const;
public:
  std::vector<GLushort> sideIndexList_;
  std::vector<GLushort> bottomIndexList_;
};

class WireTeapot : public WirePrimitiveModel {
public:
  WireTeapot(GLfloat size, const matsu::vec4 &color);
  virtual ~WireTeapot();
  const std::vector<DrawCommand> getDrawCommand() const;
private:
  std::vector<GLushort> indexList_;
};

class SolidTeapot : public SolidPrimitiveModel {
public:
  SolidTeapot(GLfloat size, const matsu::vec4 &color);
  virtual ~SolidTeapot();
  const std::vector<DrawCommand> getDrawCommand() const;
};

class WireCylinder : public WirePrimitiveModel {
public:
  WireCylinder(GLfloat radius, GLfloat height, GLint slices, const matsu::vec4 &color);
  virtual ~WireCylinder();
  const std::vector<DrawCommand> getDrawCommand() const;
private:
  std::vector<GLushort> indexList_;
};

class SolidCylinder : public SolidPrimitiveModel {
public:
  SolidCylinder(GLfloat radius, GLfloat height, GLint slices, const matsu::vec4 &color);
  virtual ~SolidCylinder();
  const std::vector<DrawCommand> getDrawCommand() const;
private:
  std::vector<GLushort> topIndexList_;
  std::vector<GLushort> bottomIndexList_;
  std::vector<GLushort> sideIndexList_;
};

class WireAxis : public WirePrimitiveModel {
public:
  WireAxis(GLfloat size, const matsu::vec4 &xColor, const matsu::vec4 &yColor, const matsu::vec4 &zColor);
  WireAxis(GLfloat size);
  virtual ~WireAxis();
  const std::vector<DrawCommand> getDrawCommand() const;
private:
  void init(float size, const matsu::vec4 &xColor, const matsu::vec4 &yColor, const matsu::vec4 &zColor);
  std::vector<GLushort> indexList_;
};
#endif
}

#endif  // SORA_PRIMITIVE_MODEL_H_