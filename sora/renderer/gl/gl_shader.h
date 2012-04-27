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
#ifndef SORA_GL_SHADER_H_
#define SORA_GL_SHADER_H_

#if SR_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#if SR_USE_PCH == 0
#include <string>
#include "core/tuple_inc.h"
#include <vector>
#endif

#include "core/assert_inc.h"
#include "renderer/globals.h"
#include "renderer/renderer_env.h"
#include "renderer/buffer_object.h"
#include "renderer/shader_variable.h"

namespace sora {;
namespace gl {
  class GLShader;
  class GLProgram;

  class GLShader {
  public:
    GLShader();
    ~GLShader();

    bool InitVertexShader(const std::vector<const char*> &src_list);
    bool InitFragmentShader(const std::vector<const char*> &src_list);
    bool InitVertexShader(const char* src) {
      std::vector<const char*> src_list;
      src_list.push_back(src);
      return InitVertexShader(src_list);
    }
    bool InitFragmentShader(const char* src) {
      std::vector<const char*> src_list;
      src_list.push_back(src);
      return InitFragmentShader(src_list);
    }

    void Deinit();
    bool IsInit() const { return (handle != 0); }

  private:
    bool InitShader(GLenum shader_type, const std::vector<const char*> &src_list);

  public:
    GLuint handle;
    GLenum type;
  };


  class GLProgram {
  public:
    typedef ShaderHandleType HandleType;
  protected:
    ~GLProgram() {}
  public:
    //policy에 연결시켜서 쓸 함수
    static bool Validate(HandleType prog);

    static bool Init(HandleType *handle, const std::string &v_src, const std::string &f_src);
    static bool Init(HandleType *handle, const std::vector<const char*> &v_src_list, const std::vector<const char*> &f_src_list);
    static void Deinit(HandleType *handle);
    static bool IsInit(const HandleType &handle) { return (handle != 0); }

    //속성 연결하기. 이름을 기반으로 연결. 검색속도가 영 느리다 싶으면
    //해시를 적용하든 뭘 하자
    //값을 1개 등록한다는건 uniform일거라는거다
    //attrib엿으면 배열이건 포인터로 넘기겟지    
    template<typename T>
    bool SetMatrix(const HandleType &handle, const ShaderVariable &var, const glm::detail::tmat4x4<T> &mat);
    template<typename T>
    bool SetMatrix(const HandleType &handle, const ShaderVariable &var, const glm::detail::tmat3x3<T> &mat);
    template<typename T>
    bool SetVector(const HandleType &handle, const ShaderVariable &var, const glm::detail::tvec4<T> &vec);
    template<typename T>
    bool SetVector(const HandleType &handle, const ShaderVariable &var, const glm::detail::tvec3<T>&vec);
    template<typename T>
    bool SetValue(const HandleType &handle, const ShaderVariable &var, T value);

    template<typename T>
    static void SetVertexList(const HandleType &handle, char *base_ptr);

    template<typename VertexContainer>
    static void SetVertexList(const HandleType &handle, const VertexContainer &vertex_data);

    //진짜로 그리기
    static void DrawArrays(const HandleType &handle, DrawType mode, int vertex_count) {
      glDrawArrays(GLEnv::TypeToGLEnum(mode), 0, vertex_count);
      SR_CHECK_ERROR("glDrawArrays");
    }

    template<typename IndexContainer>
    static void DrawElements(const HandleType &handle, DrawType mode, const IndexContainer &index_data);

    //최초실행시에는 모든목록을 얻을수잇다
    static std::vector<ShaderVariable> GetActiveUniformVarList(HandleType handle);
    static std::vector<ShaderVariable> GetActiveAttributeVarList(HandleType handle);
  private:
    static void SetPositionAttrib(HandleType handle, char *base_ptr, const VertexInfo &info);
    static void SetTexcoordAttrib(HandleType handle, char *base_ptr, const VertexInfo &info);
    static void SetNormalAttrib(HandleType handle, char *base_ptr, const VertexInfo &info);
    static void SetColorAttrib(HandleType handle, char *base_ptr, const VertexInfo &info);
    static void SetTangentAttrib(HandleType handle, char *base_ptr, const VertexInfo &info);
  };

  template<typename T>
  void GLProgram::SetVertexList(const HandleType &handle, char *base_ptr) {
    VertexInfo &info = GLEnv::GetGLVertexInfo<T>();
    SetPositionAttrib(handle, base_ptr, info);
    SetTexcoordAttrib(handle, base_ptr, info);
    SetNormalAttrib(handle, base_ptr, info);
    SetColorAttrib(handle, base_ptr, info);
    SetTangentAttrib(handle, base_ptr, info);
  }

  template<typename VertexContainer>
  void GLProgram::SetVertexList(const HandleType &handle, const VertexContainer &vertex_data) {
    typedef typename VertexContainer::value_type VertexType;
    typedef VertexBufferInfoHolder<VertexContainer> InfoHolder;
    unsigned int buffer = InfoHolder::buffer(vertex_data);
    const bool is_buffer = InfoHolder::is_buffer;
    char *ptr = (char*)vertex_data.data();

    if(is_buffer) {
      glBindBuffer(GL_ARRAY_BUFFER, buffer);
    }
    SetVertexList<VertexType>(handle, ptr);
    if(is_buffer) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }

  template<typename IndexContainer>
  void GLProgram::DrawElements(const HandleType &var, DrawType mode, const IndexContainer &index_data) {
    GLenum draw_mode = GLEnv::TypeToGLEnum(mode);
    const bool is_buffer = IndexBufferInfoHolder<IndexContainer>::is_buffer;
    unsigned int buffer = IndexBufferInfoHolder<IndexContainer>::buffer(index_data);
    if(is_buffer) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    }
    glDrawElements(draw_mode, index_data.size(), GL_UNSIGNED_SHORT, index_data.data());
    if(is_buffer) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    SR_CHECK_ERROR("glDrawElements");
  }

  template<typename T>
  bool GLProgram::SetMatrix(const HandleType &handle, const ShaderVariable &var, const glm::detail::tmat4x4<T> &mat) {
    SR_ASSERT(var.location != -1 && "not valid shader var location");
    SR_ASSERT(var.location_type == kHandleUniform);
    SR_ASSERT(var.size == 1);

    const bool is_float_type = std::is_same<T, float>::value;
    static_assert(is_float_type, "only uniform matrix support float");

    if(is_float_type) {
      GLenum var_type = GLEnv::VarTypeToGLEnum(var.var_type);
      SR_ASSERT(var_type == GL_FLOAT_MAT4);
      glUniformMatrix4fv(var.location, 1, GL_FALSE, glm::value_ptr(mat));
      SR_CHECK_ERROR("glUniformMatrix4fv");
      return true;
    }
    SR_ASSERT(!"do not reach");
    return false;
  }
  template<typename T>
  bool GLProgram::SetMatrix(const HandleType &handle, const ShaderVariable &var, const glm::detail::tmat3x3<T> &mat) {
    SR_ASSERT(var.location != -1 && "not valid shader var location");
    SR_ASSERT(var.location_type == kHandleUniform);
    SR_ASSERT(var.size == 1);

    const bool is_float_type = std::is_same<T, float>::value;
    static_assert(is_float_type, "only uniform matrix support float");

    if(is_float_type) {
      GLenum var_type = GLEnv::VarTypeToGLEnum(var.var_type);
      SR_ASSERT(var_type == GL_FLOAT_MAT3);
      glUniformMatrix3fv(var.location, 1, GL_FALSE, glm::value_ptr(mat));
      return true;
    }
    SR_ASSERT(!"do not reach");
    return false;
  }

  template<typename T>
  bool GLProgram::SetVector(const HandleType &handle, const ShaderVariable &var, const glm::detail::tvec4<T> &vec) {
    SR_ASSERT(var.location != -1 && "not valid shader var location");
    SR_ASSERT(var.location_type == kHandleUniform);
    SR_ASSERT(var.size == 1);

    const bool is_float_type = std::is_same<T, float>::value;
    const bool is_int_type = std::is_same<T, int>::value;
    static_assert(is_float_type || is_int_type, "vec4 support int, float");

    void *ptr = (void*)glm::value_ptr(vec);
    GLenum var_type = GLEnv::VarTypeToGLEnum(var.var_type);
    if(is_float_type) {
      SR_ASSERT(var_type == GL_FLOAT_VEC4);
      glUniform4fv(var.location, 1, (float*)ptr);
      return true;
    } else if(is_int_type) {
      SR_ASSERT(var_type == GL_INT_VEC4);
      glUniform4iv(var.location, 1, (int*)ptr);
      return true;
    } else {
      SR_ASSERT(!"do not reach");
      return false;
    }
  }
  template<typename T>
  bool GLProgram::SetVector(const HandleType &handle, const ShaderVariable &var, const glm::detail::tvec3<T>&vec) {
    SR_ASSERT(var.location != -1 && "not valid shader var location");
    SR_ASSERT(var.location_type == kHandleUniform);
    SR_ASSERT(var.size == 1);

    const bool is_float_type = std::is_same<T, float>::value;
    const bool is_int_type = std::is_same<T, int>::value;
    static_assert(is_float_type || is_int_type, "vec3 support int, float");

    void *ptr = (void*)glm::value_ptr(vec);
    GLenum var_type = GLEnv::VarTypeToGLEnum(var.var_type);
    if(is_float_type) {
      SR_ASSERT(var_type == GL_FLOAT_VEC3);
      glUniform3fv(var.location, 1, (float*)ptr);
      return true;
    } else if(is_int_type) {
      SR_ASSERT(var_type == GL_INT_VEC3);
      glUniform3iv(var.location, 1, (int*)ptr);
      return true;
    } else {
      SR_ASSERT(!"do not reach");
      return false;
    }
  }

  template<typename T>
  bool GLProgram::SetValue(const HandleType &handle, const ShaderVariable &var, T value) {
    SR_ASSERT(var.location != -1 && "not valid shader var location");
    SR_ASSERT(var.location_type == kHandleUniform);
    SR_ASSERT(var.size == 1);

    const bool is_float_type = std::is_same<T, float>::value;
    const bool is_int_type = std::is_same<T, int>::value;
    static_assert(is_float_type || is_int_type, "support int, float");
    GLenum var_type = GLEnv::VarTypeToGLEnum(var.var_type);

    if(std::tr1::is_same<T, float>::value) {
      SR_ASSERT(var_type == GL_FLOAT);
      glUniform1f(var.location, value);
      return true;
    } else if(std::tr1::is_same<T, int>::value) {
      SR_ASSERT(var_type == GL_INT);
      glUniform1i(var.location, value);
      return true;
    } else {
      SR_ASSERT(!"do not reach")
        return false;
    }
  }

} //namespace gl
} //namespace sora
#endif  // SORA_SHADER_H_