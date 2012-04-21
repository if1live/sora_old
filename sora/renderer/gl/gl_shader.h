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
  struct GLHandle;

  struct GLHandle {
  public:
    GLHandle();
    void Set(int var_type, HandleType loc_type, const char *attr_name, int size);

    bool operator==(const GLHandle &o) const;
    bool operator!=(const GLHandle &o) const;

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
    static bool Validate(GLuint prog);

  public:
    GLProgram();
    ~GLProgram();

    bool Init(const char *v_src, const char *f_src);
    bool Init(const std::string &v_src, const std::string &f_src) {
      return Init(v_src.c_str(), f_src.c_str());
    }
    bool Init(const std::vector<const char*> &v_src_list, const std::vector<const char*> &f_src_list);
    

    void Deinit();
    bool IsInit() const { return (prog != 0); }

    GLint GetAttribLocation(const char *name) const;
    GLint GetUniformLocation(const char *name) const;

    //최초실행시에는 모든목록을 얻을수잇다
    std::vector<GLHandle> GetActiveUniformVarList();
    std::vector<GLHandle> GetActiveAttributeVarList();

    const GLHandle *uniform_var(const char *name) const {
      return uniform_var(std::string(name));
    }
    const GLHandle *attrib_var(const char *name) const {
      return attrib_var(std::string(name));
    }
    const GLHandle *uniform_var(const std::string &name) const;
    const GLHandle *attrib_var(const std::string &name) const;
    //uniform, attrib가리지 않고 얻기. 핸들안에 타입정보가 잇으니
    //얻기만 하면 어떻게든 된다
    const GLHandle *GetHandle(const std::string &name) const;

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
    HandleType SetMatrix(const GLHandle &handle, const glm::detail::tmat4x4<T> &mat);
    template<typename T>
    HandleType SetMatrix(const GLHandle &handle, const glm::detail::tmat3x3<T> &mat);
    template<typename T>
    HandleType SetVector(const GLHandle &handle, const glm::detail::tvec4<T> &vec);
    template<typename T>
    HandleType SetVector(const GLHandle &handle, const glm::detail::tvec3<T>&vec);
    template<typename T>
    HandleType SetValue(const GLHandle &handle, T value);
    
    //connect vertex attrib
    
    void SetVertexList(char *base_ptr, Type2Type<TangentVertex>);
    void SetVertexList(char *base_ptr, Type2Type<Vertex>);
    void SetVertexList(char *base_ptr, Type2Type<Vertex2D>);

    template<typename VertexType>
    void SetVertexList(const std::vector<VertexType> &vert_list);
    template<typename VertexType, typename BaseBufferType>
    void SetVertexList(const VertexBufferObjectT<BaseBufferType, VertexType> &vbo);

    //진짜로 그리기
    void DrawArrays(DrawType mode, int vertex_count) {
      glDrawArrays(GLEnv::TypeToGLEnum(mode), 0, vertex_count);
      SR_CHECK_ERROR("glDrawArrays");
    }
    void DrawElements(DrawType mode, const IndexList &index_list) {
      glDrawElements(GLEnv::TypeToGLEnum(mode), index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);
      SR_CHECK_ERROR("glDrawElements");
    }
    template<typename BaseBufferType>
    void DrawElements(DrawType mode, const IndexBufferObjectT<BaseBufferType> &ibo);

    template<typename VertexType> void SetPositionAttrib(char *base_ptr);
    template<typename VertexType> void SetTexcoordAttrib(char *base_ptr);
    template<typename VertexType> void SetNormalAttrib(char *base_ptr);
    template<typename VertexType> void SetColorAttrib(char *base_ptr);
    template<typename VertexType> void SetTangentAttrib(char *base_ptr);
  public:
    GLuint prog;

  private:
    GLShader vert_shader_;
    GLShader frag_shader_;

    std::vector<GLHandle> uniform_var_list_;
    std::vector<GLHandle> attrib_var_list_;
    const GLHandle *FindShaderVar(const std::string &name, const std::vector<GLHandle> &var_list) const;
  };

  //impl
  template<typename T>
  HandleType GLProgram::SetMatrix(const std::string &name, const glm::detail::tmat4x4<T> &mat) {
    const GLHandle *handle = uniform_var(name);
    if(handle == NULL) {
      return kHandleNone;
    }
    return SetMatrix(*handle, mat);
  }

  template<typename T>
  HandleType GLProgram::SetMatrix(const std::string &name, const glm::detail::tmat3x3<T> &mat) {
    const GLHandle *handle = uniform_var(name);
    if(handle == NULL) {
      return kHandleNone;
    }
    return SetMatrix(*handle, mat);
  }

  template<typename T>
  HandleType GLProgram::SetVector(const std::string &name, const glm::detail::tvec4<T> &vec) {
    const GLHandle *handle = uniform_var(name);
    if(handle == NULL) {
      return kHandleNone;
    }
    return SetVector(*handle, vec);
  }
  template<typename T>
  HandleType GLProgram::SetVector(const std::string &name, const glm::detail::tvec3<T>&vec) {
    const GLHandle *handle = uniform_var(name);
    if(handle == NULL) {
      return kHandleNone;
    }
    return SetVector(*handle, vec);
  }

  template<typename T>
  HandleType GLProgram::SetValue(const std::string &name, T value) {
    const GLHandle *handle = uniform_var(name);
    if(handle == NULL) {
      return kHandleNone;
    }
    return SetVector(*handle, value);
  }

  template<typename T>
  HandleType GLProgram::SetMatrix(const GLHandle &handle, const glm::detail::tmat4x4<T> &mat) {
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
  HandleType GLProgram::SetMatrix(const GLHandle &handle, const glm::detail::tmat3x3<T> &mat) {
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
  HandleType GLProgram::SetVector(const GLHandle &handle, const glm::detail::tvec4<T> &vec) {
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
  HandleType GLProgram::SetVector(const GLHandle &handle, const glm::detail::tvec3<T>&vec) {
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
  HandleType GLProgram::SetValue(const GLHandle &handle, T value) {
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

  template<typename BaseBufferType>
  void GLProgram::DrawElements(DrawType mode, const IndexBufferObjectT<BaseBufferType> &ibo) {
    GLenum draw_mode = GLEnv::TypeToGLEnum(mode);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.ibo().buffer());
    glDrawElements(draw_mode, ibo.count(), GL_UNSIGNED_SHORT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  template<typename VertexType>
  void GLProgram::SetVertexList(const std::vector<VertexType> &vert_list) {
    char *ptr = (char*)&vert_list[0];
    SetVertexList(ptr, Type2Type<VertexType>());
  }
  template<typename VertexType, typename BaseBufferType>
  void GLProgram::SetVertexList(const VertexBufferObjectT<BaseBufferType, VertexType> &vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo.vbo().buffer());
    SetVertexList((char*)NULL, Type2Type<VertexType>());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  template<typename VertexType>
  void GLProgram::SetPositionAttrib(char *base_ptr) {
    const GLHandle *pos_handle = attrib_var(kPositionHandleName);
    const VertexInfo &info = VertexInfoHolder<VertexType>::Get();
    const int vertex_size = sizeof(VertexType);
    if(pos_handle != NULL) {
      int offset = info.pos_offset;
      GLenum type = info.pos_type;
      int loc = pos_handle->location;
      if(offset != -1) {
        //glEnableVertexAttribArray(pos_var.location);
        char *ptr = base_ptr + offset;
        glVertexAttribPointer(loc, 3, type, GL_FALSE, vertex_size, ptr);
        SR_CHECK_ERROR("glVertexAttribPointer");
      }
    }
  }
  template<typename VertexType>
  void GLProgram::SetTexcoordAttrib(char *base_ptr) {
    const GLHandle *pos_handle = attrib_var(kPositionHandleName);
    const VertexInfo &info = VertexInfoHolder<VertexType>::Get();
    const int vertex_size = sizeof(VertexType);
    const GLHandle *texcoord_handle = this->attrib_var(kTexcoordHandleName);
    if(texcoord_handle != NULL) {
      int offset = info.texcoord_offset;
      GLenum type = info.texcoord_type;
      int loc = texcoord_handle->location;
      if(offset != -1) {
        //glEnableVertexAttribArray(texcoord_var.location);
        char *ptr = base_ptr + offset;
        glVertexAttribPointer(loc, 2, type, GL_FALSE, vertex_size, ptr);
        SR_CHECK_ERROR("glVertexAttribPointer");
      }
    }
  }
  template<typename VertexType>
  void GLProgram::SetNormalAttrib(char *base_ptr) {
    const GLHandle *pos_handle = attrib_var(kPositionHandleName);
    const VertexInfo &info = VertexInfoHolder<VertexType>::Get();
    const int vertex_size = sizeof(VertexType);
    const GLHandle *normal_handle = this->attrib_var(kNormalHandleName);
    if(normal_handle != NULL) {
      int offset = info.normal_offset;
      GLenum type = info.normal_type;
      int loc = normal_handle->location;
      if(offset != -1) {
        char *ptr = base_ptr + offset;
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 3, type, GL_FALSE, vertex_size, ptr);
        SR_CHECK_ERROR("glVertexAttribPointer");
      }
    }
  }
  template<typename VertexType>
  void GLProgram::SetColorAttrib(char *base_ptr) {
    const GLHandle *pos_handle = attrib_var(kPositionHandleName);
    const VertexInfo &info = VertexInfoHolder<VertexType>::Get();
    const int vertex_size = sizeof(VertexType);
    const GLHandle *color_handle = this->attrib_var(kColorHandleName);
    if(color_handle != NULL) {
      int offset = info.color_offset;
      GLenum type = info.color_type;
      int loc = color_handle->location;
      if(offset != -1) {
        //색속성은 ubyte니까 normalize해야됨
        char *ptr = base_ptr + offset;
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 4, type, GL_TRUE, vertex_size, ptr);
        SR_CHECK_ERROR("glVertexAttribPointer");
      }
    }
  }
  template<typename VertexType> 
  void GLProgram::SetTangentAttrib(char *base_ptr) {
    const GLHandle *pos_handle = attrib_var(kPositionHandleName);
    const VertexInfo &info = VertexInfoHolder<VertexType>::Get();
    const int vertex_size = sizeof(VertexType);
    const GLHandle *tangent_handle = this->attrib_var(kTangentHandleName);
    if(tangent_handle != NULL) {
      int offset = info.tangent_offset;
      GLenum type = info.tangent_type;
      int loc = tangent_handle->location;
      if(offset != -1) {
        char *ptr = base_ptr + offset;
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 4, type, GL_FALSE, vertex_size, ptr);
        SR_CHECK_ERROR("glVertexAttribPointer");
      }
    }
  }

  
} //namespace gl
} //namespace sora
#endif  // SORA_SHADER_H_