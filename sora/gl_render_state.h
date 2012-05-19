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
#ifndef SORA_GL_RENDER_DEVIDE_H_
#define SORA_GL_RENDER_DEVIDE_H_

#if SR_USE_GL

#include "globals.h"
#include "gl_env.h"
#include "render_state.h"

namespace sora {;
struct Material;

namespace gl {
  class GLRenderState : public sora::RenderStateInterface {
  public:
    GLRenderState(RenderState *state);
    ~GLRenderState();

    void EndRender();

  public:
    void Set2D();
    void Set3D();
    glm::mat4 GetProjection3D() const;
    glm::mat4 GetProjection2D() const;
    void ClearBuffer(bool color, bool depth, bool stencil, const sora::vec4ub &value);

    //texture
  public:
    void UseTexture(Texture &tex, int unit);
    void UnuseTexture(int unit);
  private:
    std::array<TextureHandleType, 8> last_tex_id_list_;

    //shader
  public:
    void UseShader(Shader &shader);
  private:
    ShaderHandleType last_prog_id_;

    //material
  public:
    void UseMaterial(const Material &mtl);
    void UnuseMaterial();
    const Material &LastMaterial() const;
  private:
    std::unique_ptr<Material> last_mtl_;

  public:
    //win size
    void SetWinSize(int width, int height);
  private:
    RenderState *state_;
  };
} //namespace gl
} //namespace sora

#endif  // SR_USE_GL
#endif  // SORA_GL_RENDER_DEVIDE_H_