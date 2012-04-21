﻿/*  Copyright (C) 2011-2012 by if1live */
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

namespace sora {;
template<typename T> class ShaderT;
typedef ShaderT<sora::gl::GLProgram> Shader;

template<typename PolicyType>
class ShaderT {
public:
  ShaderT() {}
  ~ShaderT() {}

  bool LoadFromFile(const std::string &vert_path, const std::string &frag_path);
  bool Init(const char *vert_src, const char *frag_src);
  void Deinit();

  //uniform bind function
  template<typename T>
  HandleType SetMatrix(const std::string &name, const glm::detail::tmat4x4<T> &mat);
  template<typename T>
  HandleType SetMatrix(const std::string &name, const glm::detail::tmat3x3<T> &mat);
  template<typename T>
  HandleType SetVector(const std::string &name, const glm::detail::tvec4<T> &vec);
  template<typename T>
  HandleType SetVector(const std::string &name, const glm::detail::tvec3<T>&vec);
  template<typename T>
  HandleType SetValue(const std::string &name, T value);

  //attrib bind function
  template<typename VertexType>
  void DrawArrays(DrawType mode, const std::vector<VertexType> &vert_list);
  template<typename VertexType>
  void DrawElements(DrawType mode, const std::vector<VertexType> &vert_list, const IndexList &index_list);

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

template<typename PolicyType>
bool ShaderT<PolicyType>::Init(const char *vert_src, const char *frag_src) {
  return policy_.Init(vert_src, frag_src);
}

template<typename PolicyType>
void ShaderT<PolicyType>::Deinit() {
  policy_.Deinit();
}

template<typename PolicyType>
template<typename T>
HandleType ShaderT<PolicyType>::SetMatrix(const std::string &name, const glm::detail::tmat4x4<T> &mat) {
  return policy_.SetMatrix(name, mat);
}

template<typename PolicyType>
template<typename T>
HandleType ShaderT<PolicyType>::SetMatrix(const std::string &name, const glm::detail::tmat3x3<T> &mat) {
  return policy_.SetMatrix(name, mat);
}

template<typename PolicyType>
template<typename T>
HandleType ShaderT<PolicyType>::SetVector(const std::string &name, const glm::detail::tvec4<T> &vec) {
  return policy_.SetVector(name, vec);
}

template<typename PolicyType>
template<typename T>
HandleType ShaderT<PolicyType>::SetVector(const std::string &name, const glm::detail::tvec3<T>&vec) {
  return policy_.SetVector(name, vec);
}

template<typename PolicyType>
template<typename T>
HandleType ShaderT<PolicyType>::SetValue(const std::string &name, T value) {
  return policy_.SetValue(name, value);
}

template<typename PolicyType>
template<typename VertexType>
void ShaderT<PolicyType>::DrawArrays(DrawType mode, const std::vector<VertexType> &vert_list) {
  return policy_.DrawArrays(mode, vert_list);
}
template<typename PolicyType>
template<typename VertexType>
void ShaderT<PolicyType>::DrawElements(DrawType mode, const std::vector<VertexType> &vert_list, const IndexList &index_list) {
  return policy_.DrawElements(mode, vert_list, index_list);
}
} //namespace sora
#endif  // SORA_SHADER_H_