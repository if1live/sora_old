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
#ifndef SORA_SHADER_H_
#define SORA_SHADER_H_

#include "gl/gl_shader.h"
#include "buffer_object.h"

namespace sora {;
template<typename T> class ShaderT;
typedef ShaderT<sora::gl::GLProgram> Shader;

template<typename PolicyType>
class ShaderT {
public:
  typedef PolicyType Policy;
public:
  ShaderT() {}
  ~ShaderT() {}

  bool LoadFromFile(const std::string &vert_path, const std::string &frag_path);
  bool Init(const char *vert_src, const char *frag_src) {
    return Policy::Init(&handle_, vert_src, frag_src);
  }
  void Deinit() {
    Policy::Deinit(&handle_);
  }

  //uniform bind function
  template<typename T>
  HandleType SetMatrix(const std::string &name, const glm::detail::tmat4x4<T> &mat) {
    return Policy::SetMatrix(handle_, name, mat);
  }
  template<typename T>
  HandleType SetMatrix(const std::string &name, const glm::detail::tmat3x3<T> &mat) {
    return Policy::SetMatrix(handle_, name, mat);
  }
  template<typename T>
  HandleType SetVector(const std::string &name, const glm::detail::tvec4<T> &vec) {
    return Policy::SetVector(handle_, name, vec);
  }
  template<typename T>
  HandleType SetVector(const std::string &name, const glm::detail::tvec3<T>&vec) {
    return Policy::SetVector(handle_, name, vec);
  }
  template<typename T>
  HandleType SetValue(const std::string &name, T value) {
    return Policy::SetValue(handle_, name, value);
  }

  //attrib bind function
  //connect vertex attrib
  template<typename VertexContainer>
  void SetVertexList(const VertexContainer &vert_data) {
    if(vert_data.empty()) {
      return;
    }
    Policy::SetVertexList(handle_, vert_data);
  }

  void DrawArrays(DrawType mode, unsigned int vertex_count) {
    DrawArrays(mode, static_cast<int>(vertex_count));
  }
  void DrawArrays(DrawType mode, int vertex_count) {
    if(vertex_count == 0) {
      return;
    }
    return Policy::DrawArrays(handle_, mode, vertex_count);
  }
  
  template<typename IndexContainer>
  void DrawElements(DrawType mode, const IndexContainer &index_data) {
    if(index_data.size() == 0) {
      return;
    }
    Policy::DrawElements(handle_, mode, index_data);
  }

  //set vertex list + drawXXX를 붙인 조합형태
  template<typename VertexContainer>
  void DrawArrays(DrawType mode, const VertexContainer &vert_data) {
    Policy::SetVertexList(handle_, vert_data);
    Policy::DrawArrays(handle_, mode, vert_data.size());
  }

  template<typename VertexContainer, typename IndexContainer>
  void DrawElements(DrawType mode, const VertexContainer &vertex_data, const IndexContainer &index_data) {
    Policy::SetVertexList(handle_, vertex_data);
    Policy::DrawElements(handle_, mode, index_data);
  }

  const ShaderHandle &handle() const { return handle_; }
private:
  ShaderHandle handle_;
};
} //namespace sora


#include "sys/memory_file.h"

namespace sora {;
//impl
template<typename PolicyType>
bool ShaderT<PolicyType>::LoadFromFile(const std::string &vert_path, const std::string &frag_path) {
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

} //namespace sora
#endif  // SORA_SHADER_H_