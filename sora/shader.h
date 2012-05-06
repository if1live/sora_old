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

template<typename PolicyType>
class ShaderT : public PolicyType {
public:
  typedef PolicyType Policy;
  typedef sora::gl::ShaderHandleType HandleType;
public:
  ShaderT() : handle_(0) {}
  ~ShaderT() {}

  bool LoadFromFile(const std::string &vert_path, const std::string &frag_path);
  bool Init(const char *vert_src, const char *frag_src) {
    bool result = Policy::Init(&handle_, vert_src, frag_src);
    AfterInit();
    return result;
  }

  bool Init(const std::vector<const char*> &vert_src_list, const std::vector<const char*> &frag_src_list) {
    bool result = Policy::Init(&handle_, vert_src_list, frag_src_list);
    AfterInit();
    return result;
  }

  void AfterInit() {
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

  void Deinit() {
    Policy::Deinit(&handle_);
  }
  
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

  bool Validate() const {
    return Policy::Validate(handle_);
  }

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
  bool SetUniformValue(const std::string &name, T value) {
    return Policy::SetUniformValue(handle_, name, value);
  }

  void DrawMeshIgnoreMaterial(Mesh *mesh);

private:
  HandleType handle_;
  std::vector<ShaderVariable> uniform_list_;
  std::vector<ShaderVariable> attrib_list_;
};
} //namespace sora


#include "memory_file.h"

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

//connect vertex attrib
template<typename PolicyType>
template<typename VertexContainer>
void ShaderT<PolicyType>::SetVertexList(const VertexContainer &vert_data) {
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

template<typename PolicyType>
void ShaderT<PolicyType>::DrawArrays(DrawType mode, unsigned int vertex_count) {
  DrawArrays(mode, static_cast<int>(vertex_count));
}

template<typename PolicyType>
void ShaderT<PolicyType>::DrawArrays(DrawType mode, int vertex_count) {
  if(vertex_count > 0) {
    return Policy::DrawArrays(mode, vertex_count);
  }
}

template<typename PolicyType>
template<typename IndexContainer>
void ShaderT<PolicyType>::DrawElements(DrawType mode, const IndexContainer &index_data) {
  if(index_data.size() > 0) {
    Policy::DrawElements(mode, index_data);
  }
}

//set vertex list + drawXXX를 붙인 조합형태
template<typename PolicyType>
template<typename VertexContainer>
void ShaderT<PolicyType>::DrawArrays(DrawType mode, const VertexContainer &vert_data) {
  SetVertexList(vert_data);
  Policy::DrawArrays(mode, vert_data.size());
}

template<typename PolicyType>
template<typename VertexContainer, typename IndexContainer>
void ShaderT<PolicyType>::DrawElements(DrawType mode, const VertexContainer &vertex_data, const IndexContainer &index_data) {
  SetVertexList(vertex_data);
  Policy::DrawElements(mode, index_data);
}


template<typename PolicyType>
ShaderVariable ShaderT<PolicyType>::uniform_var(const std::string &name) const {
  return FindShaderVar(name, uniform_list_);
}

template<typename PolicyType>
ShaderVariable ShaderT<PolicyType>::attrib_var(const std::string &name) const {
  return FindShaderVar(name, attrib_list_);
}

template<typename PolicyType>
ShaderVariable ShaderT<PolicyType>::GetHandle(const std::string &name) const {
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

template<typename PolicyType>
ShaderVariable ShaderT<PolicyType>::FindShaderVar(const std::string &name, const std::vector<ShaderVariable> &var_list) const {
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

template<typename PolicyType>
void ShaderT<PolicyType>::DrawMeshIgnoreMaterial(Mesh *mesh) {
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

} //namespace sora
#endif  // SORA_SHADER_H_