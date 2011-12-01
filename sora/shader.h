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
#ifndef SORA_SHADER_H_
#define SORA_SHADER_H_

namespace sora {;
class Shader {
public:
  Shader();
  ~Shader();

  boolean InitVertexShader(const char *src);
  boolean InitFragmentShader(const char *src);
  boolean InitVertexShader(const std::string &src) {
    return InitVertexShader(src.c_str());
  }
  boolean InitFragmentShader(const std::string &src) {
    return InitFragmentShader(src.c_str());
  }
  void Deinit();
  boolean IsInit() const { return (handle != 0); }
  
private:
  boolean InitShader(GLenum shader_type, const char *src);

public:
  GLuint handle;
  GLenum type;
  std::string src;
};

class Program {
public:
  static boolean Validate(GLuint prog);
 
public:
  Program();
  ~Program();

  boolean Init(const char *v_src, const char *f_src);
  boolean Init(const std::string &v_src, const std::string &f_src) {
    return Init(v_src.c_str(), f_src.c_str());
  }
  void Deinit();
  boolean Link();
  bool IsInit() const { return (prog != 0); }

  GLint GetAttribLocation(const char *name);
  GLint GetUniformLocation(const char *name);

public:
  GLuint prog;

private:
  Shader vert_shader_;
  Shader frag_shader_;
};

}
#endif  // SORA_SHADER_H_