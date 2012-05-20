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

//#include "buffer_object.h"
#include "shader_variable.h"
#include "logger.h"

#include "gl_shader.h"
#include "gl_shader_variable.h"
#include "mesh.h"

namespace sora {;

//uniform 연결하는 전역함수
//uniform bind function
//shader variable클래스의 멤버함수로써 아래의 바인딩 함수를 넣는것을 시도한적이 잇는데 실패햇다
//원인은 shader variable은 gl shader variable를 단위정책으로 사용하기 위해서 include해야되는데
//gl shader variable은 shader variable의 내부에 잇는 정보를 참조해서 작동하기 떄문에 include해야한다
//양쪽이 서로를 include해야되는 의존성 문제로 그냥 외부에 함수로 빼냇다. 단위정책을 기반으로 이상한 짓을 할떄는
//이런식으로 전역함수같은 느낌으로 하는것이 의존성 문제를 회피하는데는 더 좋은듯하다
template<typename T>
bool SetUniformMatrix(const ShaderVariable &var, const glm::detail::tmat4x4<T> &mat) {
  return ShaderVariablePolicy::SetMatrix(var, mat);
}
template<typename T>
bool SetUniformMatrix(const ShaderVariable &var, const glm::detail::tmat3x3<T> &mat) {
  return ShaderVariablePolicy::SetMatrix(var, mat);
}
template<typename T>
bool SetUniformVector(const ShaderVariable &var, const glm::detail::tvec4<T> &vec) {
  return ShaderVariablePolicy::SetVector(var, vec);
}
template<typename T>
bool SetUniformVector(const ShaderVariable &var, const glm::detail::tvec3<T>&vec) {
  return ShaderVariablePolicy::SetVector(var, vec);
}
template<typename T>
bool SetUniformValue(const ShaderVariable &var, T value) {
  return ShaderVariablePolicy::SetValue(var, value);
}
bool SetAttrib(const ShaderVariable &var, const AttribBindParam &param, char *base_ptr);

class Shader {
public:
  typedef ShaderPolicy Policy;
  typedef ShaderHandle HandleType;

public:
  Shader();
  ~Shader();

  bool LoadFromFile(const std::string &vert_path, const std::string &frag_path);
  bool Init(const char *vert_src, const char *frag_src);
  bool Init(const std::string &vert_src, const std::string &frag_src) {
    return Init(vert_src.c_str(), frag_src.c_str());
  }
  bool Init(const std::vector<const char*> &vert_src_list, const std::vector<const char*> &frag_src_list);

  void AfterInit();

  void Deinit();
  
  //attrib bind function
  //connect vertex attrib
  template<typename VertexContainer>
  void SetVertexList(const VertexContainer &vert_data);

  static void DrawArrays(DrawType mode, unsigned int vertex_count);
  static void DrawArrays(DrawType mode, int vertex_count);
  
  template<typename IndexContainer>
  static void DrawElements(DrawType mode, const IndexContainer &index_data);

  //set vertex list + drawXXX를 붙인 조합형태
  template<typename VertexContainer>
  void DrawArrays(DrawType mode, const VertexContainer &vert_data);

  template<typename VertexContainer, typename IndexContainer>
  void DrawElements(DrawType mode, const VertexContainer &vertex_data, const IndexContainer &index_data);

  const HandleType &handle() const { return handle_; }

  //shader variable access
  ShaderVariable uniform_var(const std::string &name) const;
  ShaderVariable attrib_var(const std::string &name) const;

  //uniform, attrib가리지 않고 얻기. 핸들안에 타입정보가 잇으니
  //얻기만 하면 어떻게든 된다
  ShaderVariable GetHandle(const std::string &name) const;
  ShaderVariable FindShaderVar(const std::string &name, const std::vector<ShaderVariable> &var_list) const;


  template<typename T>
  bool SetUniformMatrix(const std::string &name, const glm::detail::tmat4x4<T> &mat) {
    return Policy::SetUniformMatrix(handle_, name, mat);
  }
  template<typename T>
  bool SetUniformMatrix(const std::string &name, const glm::detail::tmat3x3<T> &mat) {
    return Policy::SetUniformMatrix(handle_, name, mat);
  }
  template<typename T>
  bool SetUniformVector(const std::string &name, const glm::detail::tvec4<T> &vec) {
    return Policy::SetUniformVector(handle_, name, vec);
  }
  template<typename T>
  bool SetUniformVector(const std::string &name, const glm::detail::tvec3<T>&vec) {
    return Policy::SetUniformVector(handle_, name, vec);
  }
  template<typename T>
  bool SetUniformVector(const std::string &name, const glm::detail::tvec2<T>&vec) {
    return Policy::SetUniformVector(handle_, name, vec);
  }
  template<typename T>
  bool SetUniformValue(const std::string &name, T value) {
    return Policy::SetUniformValue(handle_, name, value);
  }

  void DrawMeshIgnoreMaterial(Mesh *mesh);
  bool Validate() const;

private:
  HandleType handle_;
  std::vector<ShaderVariable> uniform_list_;
  std::vector<ShaderVariable> attrib_list_;
};
} //namespace sora



namespace sora {;
//impl

//connect vertex attrib
template<typename VertexContainer>
void Shader::SetVertexList(const VertexContainer &vert_data) {
  if(vert_data.empty() == false) {
    //Policy::SetVertexList(handle_, vert_data);
    VertexListBindParam param;
    param.pos_var = attrib_var(kPositionHandleName);
    param.texcoord_var = attrib_var(kTexcoordHandleName);
    param.normal_var = attrib_var(kNormalHandleName);
    param.color_var = attrib_var(kColorHandleName);
    param.tangent_var = attrib_var(kTangentHandleName);

    Policy::SetVertexList(param, vert_data);
  }
}

template<typename IndexContainer>
void Shader::DrawElements(DrawType mode, const IndexContainer &index_data) {
  if(index_data.size() > 0) {
    Policy::DrawElements(mode, index_data);
  }
}

//set vertex list + drawXXX를 붙인 조합형태
template<typename VertexContainer>
void Shader::DrawArrays(DrawType mode, const VertexContainer &vert_data) {
  SetVertexList(vert_data);
  Policy::DrawArrays(mode, vert_data.size());
}

template<typename VertexContainer, typename IndexContainer>
void Shader::DrawElements(DrawType mode, const VertexContainer &vertex_data, const IndexContainer &index_data) {
  SetVertexList(vertex_data);
  Policy::DrawElements(mode, index_data);
}

} //namespace sora
#endif  // SORA_SHADER_H_