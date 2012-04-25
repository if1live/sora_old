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
#include "gl_vertex.h"
#include "renderer/renderer_env.h"
#include "renderer/buffer_object.h"

namespace sora {;
namespace gl {
  class GLShader;
  class GLProgram;
  struct GLShaderVarHandle;

  struct GLShaderVarHandle {
  public:
    GLShaderVarHandle();
    void Set(int var_type, HandleType loc_type, const char *attr_name, int size);

    bool operator==(const GLShaderVarHandle &o) const;
    bool operator!=(const GLShaderVarHandle &o) const;

    std::string str() const;

  public:
    int var_type; //vec2? mat4?
    HandleType location_type;  //attrib/uniform
    std::string name;  //shader코드에서의 변수이름
    int size; //속성의 크기. 쉐이더에서 배열이 아니면 전부1이다
    GLint location; //쉐이더에서의 위치값. 연결안되잇으면 일단 -1
  };


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
    //policy에 연결시켜서 쓸 함수
    static bool Validate(GLuint prog);
    static bool Validate(const ShaderHandle &handle);

    static bool Init(ShaderHandle *handle, const std::string &v_src, const std::string &f_src);
    static void Deinit(ShaderHandle *handle);
    static bool IsInit(const ShaderHandle &handle);

    template<typename T>
    static HandleType SetMatrix(const ShaderHandle &handle, const std::string &name, const glm::detail::tmat4x4<T> &mat) {
      GLProgram prog(handle.handle);
      return prog.SetMatrix(name, mat);
    }
    template<typename T>
    static HandleType SetMatrix(const ShaderHandle &handle, const std::string &name, const glm::detail::tmat3x3<T> &mat) {
      GLProgram prog(handle.handle);
      return prog.SetMatrix(name, mat);
    }
    template<typename T>
    static HandleType SetVector(const ShaderHandle &handle, const std::string &name, const glm::detail::tvec4<T> &vec) {
      GLProgram prog(handle.handle);
      return prog.SetVector(name, vec);
    }
    template<typename T>
    static HandleType SetVector(const ShaderHandle &handle, const std::string &name, const glm::detail::tvec3<T>&vec) {
      GLProgram prog(handle.handle);
      return prog.SetVector(name, vec);
    }

    template<typename T>
    static HandleType SetValue(const ShaderHandle &handle, const std::string &name, T value) {
      GLProgram prog(handle.handle);
      return prog.SetValue(name, value);
    }
    template<typename VertexContainer>
    static void SetVertexList(const ShaderHandle &handle, const VertexContainer &vertex_data) {
      GLProgram prog(handle.handle);
      prog.SetVertexList(vertex_data);
    }

    //진짜로 그리기
    static void DrawArrays(const ShaderHandle &handle, DrawType mode, int vertex_count) {
      GLProgram prog(handle.handle);
      prog.DrawArrays(mode, vertex_count);
    }

    template<typename IndexContainer>
    static void DrawElements(const ShaderHandle &handle, DrawType mode, const IndexContainer &index_data) {
      GLProgram prog(handle.handle);
      prog.DrawElements(mode, index_data);
    }

  public:
    GLProgram();
    GLProgram(GLuint prog);
    ~GLProgram();

    bool Init(const char *v_src, const char *f_src);
    bool Init(const std::string &v_src, const std::string &f_src) {
      return Init(v_src.c_str(), f_src.c_str());
    }
    bool Init(const std::vector<const char*> &v_src_list, const std::vector<const char*> &f_src_list);


    void Deinit();
    bool IsInit() const {
      return (prog != 0); 
    }

    GLint GetAttribLocation(const std::string &name) const {
      return GetAttribLocation(name.c_str());
    }
    GLint GetUniformLocation(const std::string &name) const {
      return GetUniformLocation(name.c_str());
    }
    GLint GetAttribLocation(const char *name) const;
    GLint GetUniformLocation(const char *name) const;

    //최초실행시에는 모든목록을 얻을수잇다
    std::vector<GLShaderVarHandle> GetActiveUniformVarList() const;
    std::vector<GLShaderVarHandle> GetActiveAttributeVarList() const;

    GLShaderVarHandle uniform_var(const char *name) const {
      return uniform_var(std::string(name));
    }
    GLShaderVarHandle attrib_var(const char *name) const {
      return attrib_var(std::string(name));
    }
    GLShaderVarHandle uniform_var(const std::string &name) const;
    GLShaderVarHandle attrib_var(const std::string &name) const;
    //uniform, attrib가리지 않고 얻기. 핸들안에 타입정보가 잇으니
    //얻기만 하면 어떻게든 된다
    GLShaderVarHandle GetHandle(const std::string &name) const;

    //속성 연결하기. 이름을 기반으로 연결. 검색속도가 영 느리다 싶으면
    //해시를 적용하든 뭘 하자
    //값을 1개 등록한다는건 uniform일거라는거다
    //attrib엿으면 배열이건 포인터로 넘기겟지
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

    template<typename T>
    HandleType SetMatrix(const GLShaderVarHandle &handle, const glm::detail::tmat4x4<T> &mat);
    template<typename T>
    HandleType SetMatrix(const GLShaderVarHandle &handle, const glm::detail::tmat3x3<T> &mat);
    template<typename T>
    HandleType SetVector(const GLShaderVarHandle &handle, const glm::detail::tvec4<T> &vec);
    template<typename T>
    HandleType SetVector(const GLShaderVarHandle &handle, const glm::detail::tvec3<T>&vec);
    template<typename T>
    HandleType SetValue(const GLShaderVarHandle &handle, T value);

    template<typename T>
    HandleType SetMatrix(GLuint loc, const glm::detail::tmat4x4<T> &mat);
    template<typename T>
    HandleType SetMatrix(GLuint loc, const glm::detail::tmat3x3<T> &mat);
    template<typename T>
    HandleType SetVector(GLuint loc, const glm::detail::tvec4<T> &vec);
    template<typename T>
    HandleType SetVector(GLuint loc, const glm::detail::tvec3<T>&vec);
    template<typename T>
    HandleType SetValue(GLuint loc, T value);

    //connect vertex attrib
    template<typename T>
    void SetVertexList(char *base_ptr);

    template<typename VertexContainer>
    void SetVertexList(const VertexContainer &vertex_data) {
      typedef typename VertexContainer::value_type VertexType;
      typedef VertexBufferInfoHolder<VertexContainer> InfoHolder;
      unsigned int buffer = InfoHolder::buffer(vertex_data);
      const bool is_buffer = InfoHolder::is_buffer;
      char *ptr = (char*)vertex_data.data();

      if(is_buffer) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
      }
      SetVertexList<VertexType>(ptr);
      if(is_buffer) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
      }
    }

    //진짜로 그리기
    void DrawArrays(DrawType mode, int vertex_count) {
      glDrawArrays(GLEnv::TypeToGLEnum(mode), 0, vertex_count);
      SR_CHECK_ERROR("glDrawArrays");
    }

    template<typename IndexContainer>
    void DrawElements(DrawType mode, const IndexContainer &index_data) {
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

  private:
    void SetPositionAttrib(char *base_ptr, const VertexInfo &info);
    void SetTexcoordAttrib(char *base_ptr, const VertexInfo &info);
    void SetNormalAttrib(char *base_ptr, const VertexInfo &info);
    void SetColorAttrib(char *base_ptr, const VertexInfo &info);
    void SetTangentAttrib(char *base_ptr, const VertexInfo &info);
  public:
    GLuint prog;

  private:
    GLShaderVarHandle FindShaderVar(const std::string &name, const std::vector<GLShaderVarHandle> &var_list) const;
  };

  //impl
  template<typename T>
  HandleType GLProgram::SetMatrix(const std::string &name, const glm::detail::tmat4x4<T> &mat) {
    int loc = GetUniformLocation(name);
    if(loc == -1) {
      return kHandleNone;
    }
    return SetMatrix(loc, mat);
  }

  template<typename T>
  HandleType GLProgram::SetMatrix(const std::string &name, const glm::detail::tmat3x3<T> &mat) {
    int loc = GetUniformLocation(name);
    if(loc == -1) {
      return kHandleNone;
    }
    return SetMatrix(loc, mat);
  }

  template<typename T>
  HandleType GLProgram::SetVector(const std::string &name, const glm::detail::tvec4<T> &vec) {
    int loc = GetUniformLocation(name);
    if(loc == -1) {
      return kHandleNone;
    }
    return SetVector(loc, vec);
  }
  template<typename T>
  HandleType GLProgram::SetVector(const std::string &name, const glm::detail::tvec3<T>&vec) {
    int loc = GetUniformLocation(name);
    if(loc == -1) {
      return kHandleNone;
    }
    return SetVector(loc, vec);
  }

  template<typename T>
  HandleType GLProgram::SetValue(const std::string &name, T value) {
    int loc = GetUniformLocation(name);
    if(loc == -1) {
      return kHandleNone;
    }
    return SetVector(loc, value);
  }

  template<typename T>
  HandleType GLProgram::SetMatrix(const GLShaderVarHandle &handle, const glm::detail::tmat4x4<T> &mat) {
    SR_ASSERT(handle.location_type == kHandleUniform);
    SR_ASSERT(handle.size == 1);

    const bool is_float_type = std::is_same<T, float>::value;
    static_assert(is_float_type, "only uniform matrix support float");

    if(is_float_type) {
      SR_ASSERT(handle.var_type == GL_FLOAT_MAT4);
      glUniformMatrix4fv(handle.location, 1, GL_FALSE, glm::value_ptr(mat));
      SR_CHECK_ERROR("glUniformMatrix4fv");
      return kHandleUniform;
    }
    SR_ASSERT(!"do not reach");
    return kHandleNone;
  }
  template<typename T>
  HandleType GLProgram::SetMatrix(const GLShaderVarHandle &handle, const glm::detail::tmat3x3<T> &mat) {
    SR_ASSERT(handle.location_type == kHandleUniform);
    SR_ASSERT(handle.size == 1);

    const bool is_float_type = std::is_same<T, float>::value;
    static_assert(is_float_type, "only uniform matrix support float");

    if(is_float_type) {
      SR_ASSERT(handle.var_type == GL_FLOAT_MAT3);
      glUniformMatrix3fv(handle.location, 1, GL_FALSE, glm::value_ptr(mat));
      return kHandleUniform;
    }

    SR_ASSERT(!"do not reach");
    return kHandleNone;
  }

  template<typename T>
  HandleType GLProgram::SetVector(const GLShaderVarHandle &handle, const glm::detail::tvec4<T> &vec) {
    SR_ASSERT(handle.location_type == kHandleUniform);
    SR_ASSERT(handle.size == 1);

    const bool is_float_type = std::is_same<T, float>::value;
    const bool is_int_type = std::is_same<T, int>::value;
    static_assert(is_float_type || is_int_type, "vec4 support int, float");

    void *ptr = (void*)glm::value_ptr(vec);
    if(is_float_type) {
      SR_ASSERT(handle.var_type == GL_FLOAT_VEC4);
      glUniform4fv(handle.location, 1, (float*)ptr);
      return kHandleUniform;
    } else if(is_int_type) {
      SR_ASSERT(handle.var_type == GL_INT_VEC4);
      glUniform4iv(handle.location, 1, (int*)ptr);
      return kHandleUniform;
    } else {
      SR_ASSERT(!"do not reach");
      return kHandleNone;
    }
  }
  template<typename T>
  HandleType GLProgram::SetVector(const GLShaderVarHandle &handle, const glm::detail::tvec3<T>&vec) {
    SR_ASSERT(handle.location_type == kHandleUniform);
    SR_ASSERT(handle.size == 1);

    const bool is_float_type = std::is_same<T, float>::value;
    const bool is_int_type = std::is_same<T, int>::value;
    static_assert(is_float_type || is_int_type, "vec3 support int, float");

    void *ptr = (void*)glm::value_ptr(vec);
    if(is_float_type) {
      SR_ASSERT(handle.var_type == GL_FLOAT_VEC3);
      glUniform3fv(handle.location, 1, (float*)ptr);
      return kHandleUniform;
    } else if(is_int_type) {
      SR_ASSERT(handle.var_type == GL_INT_VEC3);
      glUniform3iv(handle.location, 1, (int*)ptr);
      return kHandleUniform;
    } else {
      SR_ASSERT(!"do not reach");
      return kHandleNone;
    }
  }

  template<typename T>
  HandleType GLProgram::SetValue(const GLShaderVarHandle &handle, T value) {
    SR_ASSERT(handle.location_type == kHandleUniform);
    SR_ASSERT(handle.size == 1);

    const bool is_float_type = std::is_same<T, float>::value;
    const bool is_int_type = std::is_same<T, int>::value;
    static_assert(is_float_type || is_int_type, "support int, float");

    if(std::tr1::is_same<T, float>::value) {
      SR_ASSERT(handle.var_type == GL_FLOAT);
      glUniform1f(handle.location, value);
      return kHandleUniform;
    } else if(std::tr1::is_same<T, int>::value) {
      SR_ASSERT(handle.var_type == GL_INT);
      glUniform1i(handle.location, value);
      return kHandleUniform;
    } else {
      SR_ASSERT(!"do not reach")
        return kHandleNone;
    }
  }

  template<typename T>
  HandleType GLProgram::SetMatrix(GLuint loc, const glm::detail::tmat4x4<T> &mat) {
    const bool is_float_type = std::is_same<T, float>::value;
    static_assert(is_float_type, "only uniform matrix support float");

    if(is_float_type) {
      glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
      SR_CHECK_ERROR("glUniformMatrix4fv");
      return kHandleUniform;
    }
    SR_ASSERT(!"do not reach");
    return kHandleNone;
  }
  template<typename T>
  HandleType GLProgram::SetMatrix(GLuint loc, const glm::detail::tmat3x3<T> &mat) {
    const bool is_float_type = std::is_same<T, float>::value;
    static_assert(is_float_type, "only uniform matrix support float");

    if(is_float_type) {
      glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
      return kHandleUniform;
    }

    SR_ASSERT(!"do not reach");
    return kHandleNone;
  }
  template<typename T>
  HandleType GLProgram::SetVector(GLuint loc, const glm::detail::tvec4<T> &vec) {
    const bool is_float_type = std::is_same<T, float>::value;
    const bool is_int_type = std::is_same<T, int>::value;
    static_assert(is_float_type || is_int_type, "vec4 support int, float");

    void *ptr = (void*)glm::value_ptr(vec);
    if(is_float_type) {
      glUniform4fv(loc, 1, (float*)ptr);
      return kHandleUniform;
    } else if(is_int_type) {
      glUniform4iv(loc, 1, (int*)ptr);
      return kHandleUniform;
    } else {
      SR_ASSERT(!"do not reach");
      return kHandleNone;
    }
  }
  template<typename T>
  HandleType GLProgram::SetVector(GLuint loc, const glm::detail::tvec3<T>&vec) {
    const bool is_float_type = std::is_same<T, float>::value;
    const bool is_int_type = std::is_same<T, int>::value;
    static_assert(is_float_type || is_int_type, "vec3 support int, float");

    void *ptr = (void*)glm::value_ptr(vec);
    if(is_float_type) {
      glUniform3fv(loc, 1, (float*)ptr);
      return kHandleUniform;
    } else if(is_int_type) {
      glUniform3iv(loc, 1, (int*)ptr);
      return kHandleUniform;
    } else {
      SR_ASSERT(!"do not reach");
      return kHandleNone;
    }
  }
  template<typename T>
  HandleType GLProgram::SetValue(GLuint loc, T value) {
    const bool is_float_type = std::is_same<T, float>::value;
    const bool is_int_type = std::is_same<T, int>::value;
    static_assert(is_float_type || is_int_type, "support int, float");

    if(std::tr1::is_same<T, float>::value) {
      glUniform1f(loc, value);
      return kHandleUniform;
    } else if(std::tr1::is_same<T, int>::value) {
      glUniform1i(loc, value);
      return kHandleUniform;
    } else {
      SR_ASSERT(!"do not reach")
        return kHandleNone;
    }
  }

  template<typename T>
  void GLProgram::SetVertexList(char *base_ptr) {
    VertexInfo &info = VertexInfoHolder<T>::Get();
    SetPositionAttrib(base_ptr, info);
    SetTexcoordAttrib(base_ptr, info);
    SetNormalAttrib(base_ptr, info);
    SetColorAttrib(base_ptr, info);
    SetTangentAttrib(base_ptr, info);
  }

} //namespace gl
} //namespace sora
#endif  // SORA_SHADER_H_