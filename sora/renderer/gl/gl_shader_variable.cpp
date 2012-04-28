/*  Copyright (C) 2011 by if1live */
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
#include "sora_stdafx.h"
//#include "gl_shader_variable.h"

namespace sora {;
namespace gl {
  /*
  void GLShaderVariable::SetAttrib(int loc, int offset, int vert_size, GLenum var_type, int dim, bool normalize, char *base_ptr) {
    if(offset == -1) {
      return;
    }
    if(loc != -1) {
      glEnableVertexAttribArray(loc);
      char *ptr = base_ptr + offset;
      if(normalize == false) {
        glVertexAttribPointer(loc, dim, var_type, GL_FALSE, vert_size, ptr);
      } else {
        glVertexAttribPointer(loc, dim, var_type, GL_TRUE, vert_size, ptr);
      }
      
      SR_CHECK_ERROR("glVertexAttribPointer");
    }
  }
  void GLShaderVariable::SetPositionAttrib(const ShaderVariable &var, char *base_ptr, const VertexInfo &info) {
    SetAttrib(var.location, info.pos_offset, info.size, info.pos_type, info.pos_dim, false);
  }

  void GLShaderVariable::SetTexcoordAttrib(const ShaderVariable &var, char *base_ptr, const VertexInfo &info) {
    SetAttrib(var.location, info.texcoord_offset, info.size, info.texcoord_type, info.texcoord_dim, false);
  }
  void GLShaderVariable::SetNormalAttrib(const ShaderVariable &var, char *base_ptr, const VertexInfo &info) {
    SetAttrib(var.location, info.normal_offset, info.size, info.normal_type, info.normal_dim, false);
  }

  void GLShaderVariable::SetColorAttrib(const ShaderVariable &var, char *base_ptr, const VertexInfo &info) {
    SetAttrib(var.location, info.color_offset, info.size, info.color_type, info.color_dim, true);
  }
  void GLShaderVariable::SetTangentAttrib(const ShaderVariable &var, char *base_ptr, const VertexInfo &info) {
    SetAttrib(var.location, info.tangent_offset, info.size, info.tangent_type, info.tangent_dim, false);
  }
  */

} //namespace gl
} //namespace sora
