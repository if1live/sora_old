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
  ShaderT() {}
  ~ShaderT() {}

  bool LoadFromFile(const std::string &vert_path, const std::string &frag_path);
  bool Init(const char *vert_src, const char *frag_src) {
    return policy_.Init(vert_src, frag_src);
  }
  void Deinit() {
    policy_.Deinit();
  }

  //uniform bind function
  template<typename T>
  HandleType SetMatrix(const std::string &name, const glm::detail::tmat4x4<T> &mat) {
    return policy_.SetMatrix(name, mat);
  }
  template<typename T>
  HandleType SetMatrix(const std::string &name, const glm::detail::tmat3x3<T> &mat) {
    return policy_.SetMatrix(name, mat);
  }
  template<typename T>
  HandleType SetVector(const std::string &name, const glm::detail::tvec4<T> &vec) {
    return policy_.SetVector(name, vec);
  }
  template<typename T>
  HandleType SetVector(const std::string &name, const glm::detail::tvec3<T>&vec) {
    return policy_.SetVector(name, vec);
  }
  template<typename T>
  HandleType SetValue(const std::string &name, T value) {
    return policy_.SetValue(name, value);
  }

  //attrib bind function
  //connect vertex attrib
  template<typename VertexType>
  void SetVertexList(const std::vector<VertexType> &vert_list) {
    policy_.SetVertexList(vert_list);
  }
  template<typename VertexType, typename BaseBufferType>
  void SetVertexList(const VertexBufferObjectT<BaseBufferType, VertexType> &vbo) {
    policy_.SetVertexList(vbo);
  }

  void DrawArrays(DrawType mode, int vertex_count) {
    return policy_.DrawArrays(mode, vertex_count);
  }
  void DrawElements(DrawType mode, const IndexList &index_list) {
    return policy_.DrawElements(mode, index_list);
  }
  template<typename BaseBufferType>
  void DrawElements(DrawType mode, const IndexBufferObjectT<BaseBufferType> &ibo) {
    policy_.DrawElements(mode, ibo);
  }

  //set vertex list + drawXXX를 붙인 조합형태
  template<typename VertexType>
  void DrawArrays(DrawType mode, const std::vector<VertexType> &vert_list) {
    policy_.SetVertexList(vert_list);
    policy_.DrawArrays(mode, vert_list.size());
  }
  template<typename VertexType, typename BaseBufferType>
  void DrawArrays(DrawType mode, const VertexBufferObjectT<BaseBufferType, VertexType> &vbo) {
    policy_.SetVertexList(vbo);
    policy_.DrawArrays(mode, vbo.count());
  }

  template<typename VertexType>
  void DrawElements(DrawType mode, const std::vector<VertexType> &vert_list, const IndexList &index_list) {
    policy_.SetVertexList(vert_list);
    policy_.DrawElements(mode, index_list);
  }

  template<typename VertexType, typename VBOType, typename IBOType>
  void DrawElements(DrawType mode, const VertexBufferObjectT<VBOType, VertexType> &vbo, const IndexBufferObjectT<IBOType> &ibo) {
    policy_.SetVertexList(vbo);
    policy_.DrawElements(mode, ibo);
  }

  template<typename VertexType, typename IBOType>
  void DrawElements(DrawType mode, const std::vector<VertexType> &vert_list, const IndexBufferObjectT<IBOType> &ibo) {
    policy_.SetVertexList(vert_list);
    policy_.DrawElements(mode, ibo);
  }

  template<typename VertexType, typename VBOType>
  void DrawElements(DrawType mode, const VertexBufferObjectT<VBOType, VertexType> &vbo, const IndexList &index_list) {
    policy_.SetVertexList(vbo);
    policy_.DrawElements(mode, index_list);
  }

  PolicyType &policy() { return policy_; }

private:
  PolicyType policy_;
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