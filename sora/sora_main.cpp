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
#include "sora_main.h"

//android  OpenGL ES 2.0 code based
#if SR_ANDROID
#include <jni.h>
#endif

#if SR_USE_PCH == 0
#include "gl_inc.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#endif

#include "vector.h"
#include "gl_helper.h"
#include "memory_file.h"

#include "shader.h"
#include "matrix_stack.h"

#include "obj_model.h"
#include "obj_loader.h"

using sora::GLHelper;
using sora::ShaderProgram;
using sora::MatrixStack;
using sora::ObjModel;

//fixed
GLuint tex_id = -1;

GLuint mvp_handle;
GLuint texcoord_handle;
GLuint position_handle;

ShaderProgram shader_prog;

float win_width = 0;
float win_height = 0;

ObjModel obj_model;

bool setupGraphics(int w, int h) {
  LOGI("setupGraphics(%d, %d)", w, h);
  win_width = (float)w;
  win_height = (float)h;

  //glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  float v = (float)sora::Vec2f_testFunc((float)w, (float)h);
  LOGI("testfunc %f", v);

  LOGI("Version : %s", GLHelper::GetVersion().c_str());
  LOGI("Vendor : %s", GLHelper::GetVender().c_str());
  LOGI("Renderer : %s", GLHelper::GetRenderer().c_str());
  LOGI("Extensions : %s", GLHelper::GetExtensions().c_str());

  //load shader file
  const char *vert_path = "shader/v_simple.glsl";
  const char *frag_path = "shader/f_simple.glsl";
  std::string app_vert_path = sora::Filesystem::GetAppPath(vert_path);
  std::string app_frag_path = sora::Filesystem::GetAppPath(frag_path);
  sora::MemoryFile vert_file(app_vert_path);
  sora::MemoryFile frag_file(app_frag_path);
  vert_file.Open();
  frag_file.Open();
  const char *vert_src = (const char*)(vert_file.start);
  const char *frag_src = (const char*)(frag_file.start);
  bool prog_result = shader_prog.Init(vert_src, frag_src);
  if(prog_result == false) {
    LOGE("Could not create program.");
    return false;
  }
  
  position_handle = shader_prog.GetAttribLocation("a_position");
  LOGI("glGetAttribLocation(\"a_position\") = %d", position_handle);

  
  texcoord_handle = shader_prog.GetAttribLocation("a_texcoord");
  LOGI("glGetAttribLocation(\"a_texcoord\") = %d", texcoord_handle);

  mvp_handle = shader_prog.GetUniformLocation("u_modelviewprojection");
  LOGI("GetUniformLocation(\"u_modelviewprojection\") = %d", mvp_handle);

  glViewport(0, 0, w, h);
  GLHelper::CheckError("glViewport");

  //create sample texture
  GLubyte pixel_data[4*3] = {
    255, 0, 0,
    0, 255, 0,
    0, 0, 255,
    255, 255, 0,
  };
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  //load model
  std::string path1 = sora::Filesystem::GetAppPath("obj/cube.obj");
  sora::MemoryFile file1(path1);
  file1.Open();
  sora::ObjLoader loader;
  loader.LoadObj(file1.start, file1.end, &obj_model);

  return true;
}

const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
  0.5f, -0.5f };

GLfloat texcoord_list[] = {10.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };

void renderFrame() {
  float v = sora::Vec2f_testFunc(-1, 1);	//0.0f
  texcoord_list[0] = v;

  static float rot = 0;
  rot += 0.05f;

  static float grey;
  grey += 0.01f;
  if (grey > 1.0f) {
    grey = 0.0f;
  }
  //glClearColor(grey, grey, grey, 1.0f);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  shader_prog.Use();

  glVertexAttribPointer(position_handle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
  glEnableVertexAttribArray(position_handle);

  glVertexAttribPointer(texcoord_handle, 2, GL_FLOAT, GL_FALSE, 0, texcoord_list);
  glEnableVertexAttribArray(texcoord_handle);

  MatrixStack mat_stack;
  //glm::mat4 projection = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
  //glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0));
  glm::mat4 projection = glm::perspective(60.0f, win_width / win_height, 1.0f, 100.0f);
  glm::mat4 modelview = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0), glm::vec3(0, 1, 0));
  modelview = glm::rotate(modelview, rot, glm::vec3(0, 1, 0));
  glm::mat4 mvp = projection * modelview;
  glUniformMatrix4fv(mvp_handle, 1, 0, glm::value_ptr(mvp));


  //draw cube
  glVertexAttribPointer(position_handle, 3, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex), &obj_model.vert_list[0].pos);
  glVertexAttribPointer(texcoord_handle, 2, GL_FLOAT, GL_FALSE, sizeof(sora::Vertex), &obj_model.vert_list[0].texcoord);
  glDrawElements(GL_TRIANGLES, obj_model.index_list.size(), GL_UNSIGNED_SHORT, &obj_model.index_list[0]);

  //glDrawArrays(GL_TRIANGLES, 0, 3);
}

void SORA_setup_graphics(int w, int h) {
  setupGraphics(w, h);
}

void SORA_draw_frame() {
  renderFrame();
}

SR_C_DLL void SORA_init_gl_env() {
#if SR_WIN
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }
  fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
}

void SORA_update_frame(float dt) {
}

void SORA_cleanup_graphics() {
}

#if SR_ANDROID
#include "zip_stream_file.h"
void SORA_set_apk_file_path(const char *abs_path) {
  sora::ZipStreamFile::SetApkFile(abs_path);
}
#endif