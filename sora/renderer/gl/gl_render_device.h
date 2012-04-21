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

namespace sora {;
class Device;
namespace gl {
  /*
  class TextureManager;
  class MaterialManager;
  class MeshManager;
  class UberShader;
  class Renderer;
  class Font;
  class ShaderProgram;
  */
  class GLProgram;

  class GLRenderDevice {
  public:
    GLRenderDevice(Device *dev);
    ~GLRenderDevice();

    void EndRender();
    
    //font
    //renderer
    //RenderState &render_state();
    //TextureManager &texture_mgr();
    //MaterialManager &material_mgr();
    //MeshManager &mesh_mgr();
    /*
    //uber shader + predefined shader
    ShaderProgram &uber_shader(uint flag);
    ShaderProgram &simple_shader(); //단순 2d용
    Renderer &render3d();
    Renderer &render2d();
    */
    //shader
  public:
    void UseShader(GLProgram &prog);
  private:
    GLuint last_prog_id_;

  public:
    //win size
    void SetWinSize(int width, int height);
    int win_width() const { return win_width_; }
    int win_height() const { return win_height_; }
  private:
    int win_width_;
    int win_height_;
  };
} //namespace gl
} //namespace sora
#endif  // SORA_GL_RENDER_DEVIDE_H_