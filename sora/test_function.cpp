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
#include "sora_stdafx.h"
#include "test_function.h"

#include "renderer/renderer_env.h"

#include "sys/filesystem.h"
#include "renderer/shader.h"
#include "sys/memory_file.h"
#include "renderer/texture.h"
#include "renderer/texture_manager.h"


using namespace sora;
using namespace std;

#if SR_WIN

void SORA_test_draw2(int w, int h) {
  static bool init = false;
  static ShaderProgram shader;
  static TextureManager tex_mgr;
  
  if(init == false) {
    init = true;
    //create shader
    //2d shader
    std::string app_vert_path = sora::Filesystem::GetAppPath("shader/simple.vs");
    std::string app_frag_path = sora::Filesystem::GetAppPath("shader/simple.fs");
    sora::MemoryFile vert_file(app_vert_path);
    sora::MemoryFile frag_file(app_frag_path);
    vert_file.Open();
    frag_file.Open();
    const char *vert_src = (const char*)(vert_file.start);
    const char *frag_src = (const char*)(frag_file.start);
    bool prog_result = shader.Init(vert_src, frag_src);
    if(prog_result == false) {
      LOGE("Could not create program.");
    }

    {
      std::string tex_path = sora::Filesystem::GetAppPath("texture/sora.png");
      sora::MemoryFile tex_file(tex_path);
      tex_file.Open();
      Texture tex("sora");
      tex.SetData(sora::Texture::kFilePNG, tex_file.start, tex_file.end);
      tex_mgr.Add(tex);
    }

  }
  static float a = 0;
  a += 0.1f;

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, w, h);

  SR_CHECK_ERROR("Render 2d start");
  //draw 2d something
  glm::mat4 world_mat(1.0f);

  int pos_loc = shader.GetAttribLocation("a_position");
  int tex_loc = shader.GetAttribLocation("a_texcoord");
  glEnableVertexAttribArray(pos_loc);
  glEnableVertexAttribArray(tex_loc);
  
  int mvp_loc = shader.GetUniformLocation("u_worldViewProjection");

  float vertex[] = {
    -0.5, -0.5, 0,
    0.5, -0.5, 0,
    0, 0.5, 0,
  };
  float texcoord[] = {
    0, 0,
    1, 0,
    0.5, 1,
  };


  glUseProgram(shader.prog);

  Texture *tex = tex_mgr.Get_ptr(string("sora"));
  glBindTexture(GL_TEXTURE_2D, tex->handle());

  glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(world_mat));
  glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 0, vertex);
  glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, 0, texcoord);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
  SR_CHECK_ERROR("glDrawArrays");
}

#if SR_GLES == 0

void SORA_test_draw(int w, int h) {
  static float a = 0;
  a += 0.1f;

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  float vertex[] = {
    -0.5f, -0.5f,
    0.5f, -0.5f, 
    0.0f, 0.5f
  };
  unsigned char color[] = {
    255, 0, 0,
    0, 255, 0,
    0, 0, 255,
  };

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(a, 0, 0, 1);

  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertex);
  glColorPointer(3, GL_UNSIGNED_BYTE, 0, color);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
#endif
#endif