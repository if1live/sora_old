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
#include "renderer/mesh_buffer.h"

namespace sora {;
class VertexListBindParam;

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
    static void SetVertexList(const VertexListBindParam &param, char *base_ptr);
    static void SetVertexList(const VertexListBindParam &param, char *base_ptr, const VertexInfo &info);

    template<typename T>
    static void SetVertexList(const VertexListBindParam &param, const std::vector<T> &vertex_data);
    static void SetVertexList(const VertexListBindParam &param, const VertexBufferInterface &vertex_data);

    //진짜로 그리기
    static void DrawArrays(DrawType mode, int vertex_count);
    static void DrawElements(DrawType mode, const IndexList &index_data);
    static void DrawElements(DrawType mode, const IndexBufferInterface &index_data);

    //최초실행시에는 모든목록을 얻을수잇다
    static std::vector<ShaderVariable> GetActiveUniformVarList(HandleType handle);
    static std::vector<ShaderVariable> GetActiveAttributeVarList(HandleType handle);
  };

  template<typename T>
  void GLProgram::SetVertexList(const VertexListBindParam &bind_param, char *base_ptr) {
    const VertexInfo &info = GLEnv::GetGLVertexInfo<T>();
    SetVertexList(bind_param, base_ptr, info);
  }

  template<typename T>
  void GLProgram::SetVertexList(const VertexListBindParam &param, const std::vector<T> &vertex_data) {
    char *ptr = (char*)vertex_data.data();
    SetVertexList<T>(param, ptr);
  }

  
} //namespace gl
} //namespace sora
#endif  // SORA_SHADER_H_