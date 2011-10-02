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
#ifndef RENDERING_RUNA_SHADER_PROGRAM_H_
#define RENDERING_RUNA_SHADER_PROGRAM_H_

#include <vector>
#include <string>
#include "runa/runa_enum.h"

namespace runa {;
class ShaderProgram : boost::noncopyable {
 public:
  explicit ShaderProgram(VertexShader *vert_shader,
    FragmentShader *frag_shader);
  explicit ShaderProgram(const std::string &vert_src,
    const std::string &frag_src);
  virtual ~ShaderProgram();

  bool Validate();
  bool Link();
  GLuint handle() const;

  void BindAttribLocation(GLuint index, const GLchar* name) const;
  GLuint GetAttribLocation(const GLchar* name) const;
  GLuint GetUniformLocation(const GLchar* name) const;

  void Use();
  /// @brief same with use
  void operator()();

  std::vector<ShaderLocation> GetUniformLocationList();
  std::vector<ShaderLocation> GetAttributeLocationList();

  static ShaderProgram &GetLastUsedProgram();

 private:
  bool Init();

  /// do not change shader in runtime
  /// (if you need to use new shader, create new shader program)
  void SetShader(VertexShader *vert_shader);
  void SetShader(FragmentShader *frag_shader);
  bool SetShader(VertexShader *vert_shader, FragmentShader *frag_shader);

  GLuint program_;
  std::auto_ptr<VertexShader> vertex_shader_;
  std::auto_ptr<FragmentShader> fragment_shader_;

  static ShaderProgram *last_used_prog_;
};
}
#endif  // RENDERING_RUNA_SHADER_PROGRAM_H_
