﻿/*  Copyright (C) 2011 by if1live */
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
#ifndef RENDERING_RUNA_RUNA_ENUM_H_
#define RENDERING_RUNA_RUNA_ENUM_H_

namespace runa {;
template<GLenum>
class Shader;

typedef Shader<GL_VERTEX_SHADER> VertexShader;
typedef Shader<GL_FRAGMENT_SHADER> FragmentShader;

typedef std::tr1::shared_ptr<VertexShader> VertexShaderPtr;
typedef std::tr1::shared_ptr<FragmentShader> FragmentShaderPtr;

class ShaderProgram;
typedef std::tr1::shared_ptr<ShaderProgram> ShaderProgramPtr;

class ShaderLocation;

//gl helper
class Window;
class GLTool;
class BasicColorShader;

//vertex
template<typename VertexType,
  int VertexDim,
  typename ColorType,
  int ColorDim,
  typename TexCoordType,
  int TexCoordDim>
struct Vertex;
typedef Vertex<float, 3, float, 4, float, 2> TextureVertex;
typedef Vertex<float, 3, float, 4, bool, 1> ColorVertex;

// for color
template<typename T, unsigned int D>  struct Color;
typedef Color<unsigned char, 3> Color3ub;
typedef Color<float, 3> Color3f;
typedef Color<unsigned char, 4> Color4ub;
typedef Color<float, 4> Color4f;
}

#endif  // RENDERING_RUNA_RUNA_ENUM_H_
