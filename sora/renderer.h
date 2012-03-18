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
#ifndef SORA_RENDERER_H_
#define SORA_RENDERER_H_

#include "template_lib.h"
#if SR_USE_PCH == 0
#include "gl_inc.h"
#endif

namespace sora {;

class Texture;
class Material;
class ShaderProgram;
class ObjModel;
class PrimitiveModel;

//opengles 2.0 renderer
class Renderer : public Singleton<Renderer> {
public:
  Renderer();
  ~Renderer();

  void SetWindowSize(float w, float h);
  float win_width() const { return win_width_; }
  float win_height() const { return win_height_; }

  void Set2D();
  void Set3D();

  void EndRender();

  void SetTexture(const Texture &tex);
  void SetShader(const ShaderProgram &prog);
  void SetMaterial(const Material &material);

  void DrawObj(const ObjModel &model);
  void DrawPrimitiveModel(const PrimitiveModel &model);

private:
  float win_width_;
  float win_height_;

  //like cache?
  GLuint last_tex_id_;
  GLuint last_prog_id_;
  ShaderProgram *last_prog_;
};
SR_C_DLL Renderer &Renderer_get_instance();
}

#endif  // SORA_RENDERER_H_