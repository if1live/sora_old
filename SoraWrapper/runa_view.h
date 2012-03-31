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
#ifndef SORA_RUNA_VIEW_H_
#define SORA_RUNA_VIEW_H_

//하나의 독립된 프로그램으로써 material editor를 만들기로 햇다
//코드명은 RUNA
#include "gl_view.h"
#include "renderer/uber_shader.h"

namespace sora {;
struct RunaViewPrivate;

public enum class ShaderFlag : unsigned int {
  kNone = 0,
  kConstColor = UberShader::kConstColor,
  kTexture = UberShader::kTexture,
  kAmbientColor = UberShader::kAmbientColor,
  kDiffuseColor = UberShader::kDiffuseColor,
  kSpecularColor = UberShader::kSpecularColor,
  kModelColor = UberShader::kModelColor,
  kDiffuseMap = UberShader::kDiffuseMap,
  kSpecularMap = UberShader::kSpecularMap
};

public ref class RunaView : public GLView {
public:
  RunaView();
  ~RunaView();
  virtual void SetupGraphics(int w, int h) override;
  virtual void SetWindowSize(int w, int h) override;
  virtual void DrawFrame() override;
  virtual void InitGLEnv() override;
  virtual void UpdateFrame(float dt) override;
  virtual void Cleanup() override;
  
  //material editor니까 외부에서 기본 속성을 조절할수 있어야한다. 간단하게 몇개 뚫어놓자
public:
  ShaderFlag GetShaderFlag();
  void EnableShaderFlag(ShaderFlag value);
  void DisableShaderFlag(ShaderFlag value);
  bool IsEnabledShaderFlag(ShaderFlag value);
  void SetShaderFlag(bool b, ShaderFlag value);

  void SetAmbientColor(Byte r, Byte g, Byte b);
  void SetDiffuseColor(Byte r, Byte g, Byte b);
  void SetSpecularColor(Byte r, Byte g, Byte b);
  void SetConstColor(Byte r, Byte g, Byte b);
  void SetSpecularShininess(float shininess);


  void GetAmbientColor(array<Byte> ^%color);
  void GetDiffuseColor(array<Byte> ^%color);
  void GetSpecularColor(array<Byte> ^%color);
  void GetConstColor(array<Byte> ^%color);
  float GetSpecularShininess();

  //light 속성 관련
  void SetLightMove(bool b);
  void SetLightAmbientColor(Byte r, Byte g, Byte b);
  void SetLightDiffuseColor(Byte r, Byte g, Byte b);
  void SetLightSpecularColor(Byte r, Byte g, Byte b);
  
  bool IsLightMove();
  void GetLightAmbientColor(array<Byte> ^%color);
  void GetLightDiffuseColor(array<Byte> ^%color);
  void GetLightSpecularColor(array<Byte> ^%color);

private:
  RunaViewPrivate &pimpl();
  RunaViewPrivate *pimpl_;
};
}


#endif  // SORA_RUNA_VIEW_H_