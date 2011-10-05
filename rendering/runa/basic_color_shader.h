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
/// @brief 간단한 형태의 샘플 쉐이더. gl이 잘 초기화되었나 테스트할때 사용할수있다. 
/// 단색물체만 지원한다. (디버깅용으로 유용하게 쓸거같아서 독립클래스로 만듬)
#ifndef RENDERING_RUNA_BASIC_COLOR_SHADER_H_
#define RENDERING_RUNA_BASIC_COLOR_SHADER_H_

#include "runa/runa_enum.h"
#include "sora/singleton.h"
#include <boost/noncopyable.hpp>

namespace runa {;
class BasicColorShader : public sora::Singleton<BasicColorShader>,
  private boost::noncopyable {
 public:
  BasicColorShader();
  ~BasicColorShader();

  void Initialize();
  bool IsInitialized() const;
  const runa::ShaderProgram &program() const;
  runa::ShaderProgram &program();

  void SetColor4fv(const float *ptr);
  void SetMatrix(const float *m);

  int position_location() const;
  int color_location() const;
  int mvp_location() const;
  void Use();
 private:
  std::auto_ptr<runa::ShaderProgram> program_;

  int position_location_;
  int color_location_;
  int mvp_location_;
};
}

#endif  // RENDERING_RUNA_BASIC_COLOR_SHADER_H_
