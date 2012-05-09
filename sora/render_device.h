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
#ifndef SORA_RENDER_DEVICE_H_
#define SORA_RENDER_DEVICE_H_

#include "globals.h"

namespace sora {;
struct RenderDeviceInterface;
class MatrixStack;

class RenderDevice {
public:
  RenderDevice();
  ~RenderDevice();

  void UseShader(Shader &shader);
  void UseTexture(Texture &tex);

  void Set2D();
  void Set3D();

  void EndRender();

  void SetWinSize(int width, int height);

  int win_width() const { return win_width_; }
  int win_height() const { return win_height_; }

  //matrix 
  glm::mat4 &projection_mat() { return projection_mat_; }
  glm::mat4 &view_mat() { return view_mat_; }
  MatrixStack &model_mat_stack() { return *model_mat_stack_; }
  const glm::mat4 &projection_mat() const { return projection_mat_; }
  const glm::mat4 &view_mat() const { return view_mat_; }
  const MatrixStack &model_mat_stack() const { return *model_mat_stack_; }
  const glm::mat4 &model_mat() const;

  void set_projection_mat(const glm::mat4 &m) { projection_mat_ = m; }
  void set_view_mat(const glm::mat4 &m) { view_mat_ = m; }

  //view행렬로부터 카메라 정보를 뺴올수잇다
  glm::vec3 view_up_vec() const;
  glm::vec3 view_dir_vec() const;
  glm::vec3 view_side_vec() const;


  void ResetAllMatrix();
  void ResetProjectionMat();
  void ResetViewMat();
  void ResetModelMat();

private:
  RenderDeviceInterface *policy_;
  int win_width_;
  int win_height_;

  glm::mat4 projection_mat_;
  glm::mat4 view_mat_;
  std::unique_ptr<MatrixStack> model_mat_stack_;
  //텍스쳐 좌표 스택까지 전역으로 묶을 필요가 잇을까? 해서 일단 미뤄놧다. 필요해지면 그떄 넣지뭐
};

struct RenderDeviceInterface {
  RenderDeviceInterface() {}
  virtual ~RenderDeviceInterface() {}

  virtual void UseShader(Shader &shader) = 0;
  virtual void UseTexture(Texture &tex) = 0;

  virtual void Set2D() = 0;
  virtual void Set3D() = 0;

  virtual void EndRender() = 0;

  virtual void SetWinSize(int width, int height) = 0;
};
}
#endif  // SORA_RENDER_DEVICE_H_