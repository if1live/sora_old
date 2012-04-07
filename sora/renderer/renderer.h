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
#include "light.h"
#include "material.h"

#if SR_USE_PCH == 0
#include "gl_inc.h"
#include <glm/glm.hpp>
#include <vector>
#endif

namespace sora {;

class Texture;
class Material;
class ShaderProgram;
class ObjModel;
class PrimitiveModel;
class Camera;
class MeshBufferObject;
class Light;
class Device;

template<typename VertexType> struct DrawCommand;

//2d / 3d는 미묘한 정책만 수정해도 충분할듯하다
enum {
  kRenderPolicy2D,
  kRenderPolicy3D,
};
struct RendererPolicy {
  RendererPolicy(int policy_type) : policy_type(policy_type) {}
  virtual void SetInitState() = 0;
  virtual glm::mat4 ToViewMatrixFromCamera(const Camera &cam, float win_w, float win_h) = 0;
  const int policy_type;
};
struct RendererPolicy_2D : public RendererPolicy {
  RendererPolicy_2D() : RendererPolicy(kRenderPolicy2D) {}
  void SetInitState();
  glm::mat4 ToViewMatrixFromCamera(const Camera &cam, float win_w, float win_h);
};

struct RendererPolicy_3D : public RendererPolicy {
  RendererPolicy_3D() : RendererPolicy(kRenderPolicy3D) {}
  void SetInitState();
  glm::mat4 ToViewMatrixFromCamera(const Camera &cam, float win_w, float win_h);
};

//opengles 2.0 renderer
class Renderer {
public:
  Renderer(Device *dev);
  ~Renderer();

  void SetPolicy_2D();
  void SetPolicy_3D();

  //renderer을 여러개 만든다고하더라도 사실상 공유되는 속성
public:
  //bind gl resource
  static void SetTexture(const Texture &tex);
  static void SetShader(const ShaderProgram &prog);

public:
  void SetInitState();
  static void EndRender();

  glm::mat4 &projection_mat() { return projection_; }
  glm::mat4 &view_mat() { return view_; }
  void set_projection_mat(const glm::mat4 &m) { projection_ = m; }
  void set_view_mat(const glm::mat4 &m) { view_ = m; }

  void ApplyMatrix(const glm::mat4 &world_mat);
  
  Camera &camera();
  void set_camera(const Camera &cam);

  //draw method
  /*
  template<typename T>
  void DrawSolid(const T &mesh);
  template<typename T>
  void DrawWire(const T &mesh);
  template<typename VertexType>
  void Draw(const DrawCommand<VertexType> &cmd);
  */

  void Draw(const MeshBufferObject &mesh);

  //light / material
  void SetLight(const Light &light);
  void SetMaterial(const Material &material);
  void ApplyMaterialLight();

protected:
  //access cache like data
  static GLuint last_tex_id() { return last_tex_id_; }
  static ShaderProgram *last_prog() { return last_prog_; }
  static Material &last_material() { return last_material_; }

private:
  //마지막에 잡은 opengl resource는 캐시를 공유하도록하자. 
  //사실상 상속은 이걸 위해서 등장한것에 더 가깝다
  static GLuint last_tex_id_;
  static ShaderProgram *last_prog_;
  static Material last_material_;

  //행렬의 경우, 렌더러 각각이 공유하지 않는다. 2d와 3d를 완전히 분리시킨다는 의미
  glm::mat4 projection_;
  glm::mat4 view_;

  RendererPolicy *policy_;
  Device *dev_;

  Camera cam_;
  Light light_;
  Material mtl_;
};

/*
template<typename T>
void Renderer::DrawSolid(const T &mesh) {
  std::vector<DrawCommand> cmd_list = mesh.GetDrawCmdList_solid();
  auto it = cmd_list.begin();
  auto endit = cmd_list.end();
  for( ; it != endit ; ++it) {
    Draw(*it);
  }
}

template<typename T>
void Renderer::DrawWire(const T &mesh) {
  std::vector<DrawCommand> cmd_list = mesh.GetDrawCmdList_wire();
  auto it = cmd_list.begin();
  auto endit = cmd_list.end();
  for( ; it != endit ; ++it) {
    Draw(*it);
  }
}

template<typename VertexType>
void Renderer::Draw(const DrawCommand<VertexType> &cmd) {
  if(cmd.vert_ptr == NULL) { return; }
  if(cmd.index_ptr == NULL) { return; }
  if(cmd.index_count == 0) { return; }

  const ShaderBindPolicy &bind_policy = last_prog_->bind_policy;

  const ShaderVariable &pos_var = bind_policy.var(ShaderBindPolicy::kPosition);
  const ShaderVariable &texcoord_var = bind_policy.var(ShaderBindPolicy::kTexcoord);
  const ShaderVariable &normal_var = bind_policy.var(ShaderBindPolicy::kNormal);
  const ShaderVariable &color_var = bind_policy.var(ShaderBindPolicy::kColor);

  const VertexType *vert_ptr = cmd.vert_ptr;

  //draw cube
  if(pos_var.location != -1) {
    glEnableVertexAttribArray(pos_var.location);
    glVertexAttribPointer(pos_var.location, 3, VertexType::kPosType, GL_FALSE, sizeof(VertexType), &vert_ptr->pos);
  }
  if(texcoord_var.location != 1) {
    glEnableVertexAttribArray(texcoord_var.location);
    glVertexAttribPointer(texcoord_var.location, 2, VertexType::kTexcoordType, GL_FALSE, sizeof(VertexType), &vert_ptr->texcoord);
  }
  if(normal_var.location != -1) {
    glEnableVertexAttribArray(normal_var.location);
    glVertexAttribPointer(normal_var.location, 3, VertexType::kNormalType, GL_FALSE, sizeof(VertexType), &vert_ptr->normal);
  }
  if(color_var.location != -1) {
    glEnableVertexAttribArray(color_var.location);
    glVertexAttribPointer(color_var.location, 4, VertexType::kNormalType, GL_TRUE, sizeof(VertexType), &vert_ptr->color);
  }

  int index_count = cmd.index_count;
  GLenum draw_mode = cmd.draw_mode;
  GLenum index_type = cmd.index_type;
  const void *index_ptr = cmd.index_ptr;
  glDrawElements(draw_mode, index_count, index_type, index_ptr);
}
*/
}

#endif  // SORA_RENDERER_H_