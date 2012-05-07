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
#include "shader.h"

#include "memory_file.h"

using namespace std;

namespace sora {;
bool SetAttrib(const ShaderVariable &var, const AttribBindParam &param, char *base_ptr) {
  return ShaderVariablePolicy::SetAttrib(var, param, base_ptr);
}

Shader::Shader() 
: handle_(0) {

}
Shader::~Shader() {
}

bool Shader::LoadFromFile(const std::string &vert_path, const std::string &frag_path) {
  sora::MemoryFile vert_file(vert_path);
  sora::MemoryFile frag_file(frag_path);
  vert_file.Open();
  frag_file.Open();
  const char *vert_src = (const char*)(vert_file.start);
  const char *frag_src = (const char*)(frag_file.start);
  bool prog_result = Init(vert_src, frag_src);
  if(prog_result == false) {
    LOGE("Could not create program.");
  }
  return prog_result;
}

bool Shader::Init(const char *vert_src, const char *frag_src) {
  bool result = Policy::Init(&handle_, vert_src, frag_src);
  AfterInit();
  return result;
}

bool Shader::Init(const std::vector<const char*> &vert_src_list, const std::vector<const char*> &frag_src_list) {
  bool result = Policy::Init(&handle_, vert_src_list, frag_src_list);
  AfterInit();
  return result;
}

void Shader::AfterInit() {
  uniform_list_ = Policy::GetActiveUniformVarList(handle_);
  auto it = uniform_list_.begin();
  auto endit = uniform_list_.end();
  for( ; it != endit ; ++it) {
    const ShaderVariable &loc = *it;
    LOGI("%s", loc.str().c_str());
  }

  attrib_list_ = Policy::GetActiveAttributeVarList(handle_);
  it = attrib_list_.begin();
  endit = attrib_list_.end();
  for( ; it != endit ; ++it) {
    const ShaderVariable &loc = *it;
    LOGI("%s", loc.str().c_str());
  }
}

void Shader::Deinit() {
  Policy::Deinit(&handle_);
}

void Shader::DrawMeshIgnoreMaterial(Mesh *mesh) {
  auto it = mesh->Begin();
  auto endit = mesh->End();
  for( ; it != endit ; ++it) {
    MeshBuffer *mesh_buffer = (*it)->mesh_buffer;
    auto mesh_it = mesh_buffer->begin();
    auto mesh_endit = mesh_buffer->end();
    for( ; mesh_it != mesh_endit ; ++mesh_it) {
      VertexBufferInterface *vb = mesh_buffer->VertexBuffer(mesh_it->vertex_buffer_handle);
      IndexBufferInterface *ib = mesh_buffer->IndexBuffer(mesh_it->index_buffer_handle);
      DrawType draw_mode = mesh_it->draw_mode;
      SetVertexList(*vb);

      //앞면 뒷면 그리기를 허용/불가능 정보까지 내장해야
      //뚜껑없는 원통 그리기가 편하다
      if(mesh_it->disable_cull_face == true) {
        glDisable(GL_CULL_FACE);
      }

      if(ib->empty()) {
        Shader::DrawArrays(draw_mode, vb->size());
      } else {
        Shader::DrawElements(draw_mode, *ib);
      }

      //앞면 뒷면 그리기를 허용/불가능 정보까지 내장해야
      //뚜껑없는 원통 그리기가 편하다
      if(mesh_it->disable_cull_face == true) {
        glEnable(GL_CULL_FACE);
      }
    }
  }
}

bool Shader::Validate() const {
  return Policy::Validate(handle_);
}


ShaderVariable Shader::uniform_var(const std::string &name) const {
  return FindShaderVar(name, uniform_list_);
}

ShaderVariable Shader::attrib_var(const std::string &name) const {
  return FindShaderVar(name, attrib_list_);
}

ShaderVariable Shader::GetHandle(const std::string &name) const {
  ShaderVariable handle;
  handle = uniform_var(name);
  if(handle.location != -1) {
    return handle;
  }
  handle = attrib_var(name);
  if(handle.location != -1) {
    return handle;
  }
  return handle;
}

ShaderVariable Shader::FindShaderVar(const std::string &name, const std::vector<ShaderVariable> &var_list) const {
  auto it = var_list.begin();
  auto endit = var_list.end();
  for( ; it != endit ; ++it) {
    if(it->name == name) {
      const ShaderVariable &var = *it;
      return var;
    }
  }
  ShaderVariable empty;
  return empty;
}

void Shader::DrawArrays(DrawType mode, unsigned int vertex_count) {
  DrawArrays(mode, static_cast<int>(vertex_count));
}

void Shader::DrawArrays(DrawType mode, int vertex_count) {
  if(vertex_count > 0) {
    return Policy::DrawArrays(mode, vertex_count);
  }
}


} //namespace sora