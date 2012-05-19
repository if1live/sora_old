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
#pragma once

#include "shader_variable.h"
#include "assert_inc.h"
#include "renderer_env.h"

namespace sora {;
struct ShaderVariable;
struct AttribBindParam;
namespace gl {
  class GLShaderVariable {
  public:
    //uniform bind function
    template<typename T>
    static bool SetMatrix(const ShaderVariable &var, const glm::detail::tmat4x4<T> &mat);
    template<typename T>
    static bool SetMatrix(const ShaderVariable &var, const glm::detail::tmat3x3<T> &mat);
    template<typename T>
    static bool SetVector(const ShaderVariable &var, const glm::detail::tvec4<T> &vec);
    template<typename T>
    static bool SetVector(const ShaderVariable &var, const glm::detail::tvec3<T>&vec);
    template<typename T>
    static bool SetValue(const ShaderVariable &var, T value);

    //attrib bind function
    static bool SetAttrib(const ShaderVariable &var, const AttribBindParam &param, char *base_ptr);
  };

  template<typename T>
  bool GLShaderVariable::SetMatrix(const ShaderVariable &var, const glm::detail::tmat4x4<T> &mat) {
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
  bool GLShaderVariable::SetMatrix(const ShaderVariable &var, const glm::detail::tmat3x3<T> &mat) {
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
  bool GLShaderVariable::SetVector(const ShaderVariable &var, const glm::detail::tvec4<T> &vec) {
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
  bool GLShaderVariable::SetVector(const ShaderVariable &var, const glm::detail::tvec3<T>&vec) {
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
      SR_CHECK_ERROR("glUniform3fv");
      return true;
    } else if(is_int_type) {
      SR_ASSERT(var_type == GL_INT_VEC3);
      glUniform3iv(var.location, 1, (int*)ptr);
      SR_CHECK_ERROR("glUniform3iv");
      return true;
    } else {
      SR_ASSERT(!"do not reach");
      return false;
    }
  }
  template<typename T>
  bool GLShaderVariable::SetValue(const ShaderVariable &var, T value) {
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
      SR_CHECK_ERROR("glUniform1f");
      return true;
    } else if(std::tr1::is_same<T, int>::value) {
      SR_ASSERT(var_type == GL_INT || var_type == GL_SAMPLER_2D);
      glUniform1i(var.location, value);
      SR_CHECK_ERROR("glUniform1i");
      return true;
    } else {
      SR_ASSERT(!"do not reach")
      return false;
    }
  }
} //namespace gl
} //namespace sora