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

    template<typename T>
    static void SetVertexList(const HandleType &handle, char *base_ptr);

    template<typename VertexContainer>
    static void SetVertexList(const HandleType &handle, const VertexContainer &vertex_data);

    //진짜로 그리기
    static void DrawArrays(DrawType mode, int vertex_count) {
      glDrawArrays(GLEnv::TypeToGLEnum(mode), 0, vertex_count);
      SR_CHECK_ERROR("glDrawArrays");
    }

    template<typename IndexContainer>
    static void DrawElements(DrawType mode, const IndexContainer &index_data);

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
  void GLProgram::DrawElements(DrawType mode, const IndexContainer &index_data) {
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


} //namespace gl
} //namespace sora
#endif  // SORA_SHADER_H_