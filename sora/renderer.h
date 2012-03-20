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
#ifndef SORA_RENDERER_H_
#define SORA_RENDERER_H_

#include "camera.h"
#if SR_USE_PCH == 0
#include "gl_inc.h"
#include <glm/glm.hpp>
#endif

namespace sora {;

class Texture;
class Material;
class ShaderProgram;
class ObjModel;
class PrimitiveModel;
class Camera;

//2d / 3d는 미묘한 정책만 수정해도 충분할듯하다
struct RendererPolicy {
  virtual void SetInitState() = 0;
  virtual glm::mat4 ToViewMatrixFromCamera(const Camera &cam) = 0;
};
struct RendererPolicy_2D : public RendererPolicy {
  void SetInitState();
  glm::mat4 ToViewMatrixFromCamera(const Camera &cam);
};

struct RendererPolicy_3D : public RendererPolicy {
  void SetInitState();
  glm::mat4 ToViewMatrixFromCamera(const Camera &cam);
};

//opengles 2.0 renderer
class Renderer {
public:
  static Renderer &Renderer3D();
  static Renderer &Renderer2D();
public:
  Renderer(RendererPolicy *policy);
  virtual ~Renderer();

  static void SetWindowSize(float w, float h);
  static float win_width() { return win_width_; }
  static float win_height() { return win_height_; }

  void SetInitState();
  static void EndRender();

  //bind gl resource
  void SetTexture(const Texture &tex);
  void SetShader(const ShaderProgram &prog);
  void SetMaterial(const Material &material);

  glm::mat4 &projection_mat() { return projection_; }
  glm::mat4 &view_mat() { return view_; }
  void set_projection_mat(const glm::mat4 &m) { projection_ = m; }
  void set_view_mat(const glm::mat4 &m) { view_ = m; }

  void ApplyMatrix(const glm::mat4 &world_mat);
  
  Camera &camera();
  void set_camera(const Camera &cam);

  //draw method
  void DrawObj(const ObjModel &model);
  void DrawPrimitiveModel(const PrimitiveModel &model);

protected:
  //access cache like data
  static GLuint last_tex_id() { return last_tex_id_; }
  static GLuint last_prog_id() { return last_prog_id_; }
  static ShaderProgram *last_prog() { return last_prog_; }

private:
  //다른 렌더러를 쓴다고해도 윈도우 크기는 사실상 동일
  static float win_width_;
  static float win_height_;

  //마지막에 잡은 opengl resource는 캐시를 공유하도록하자. 
  //사실상 상속은 이걸 위해서 등장한것에 더 가깝다
  static GLuint last_tex_id_;
  static GLuint last_prog_id_;
  static ShaderProgram *last_prog_;

  //행렬의 경우, 렌더러 각각이 공유하지 않는다. 2d와 3d를 완전히 분리시킨다는 의미
  glm::mat4 projection_;
  glm::mat4 view_;

  Camera cam_;

  RendererPolicy *policy_;
};
}

#endif  // SORA_RENDERER_H_