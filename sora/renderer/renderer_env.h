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
#ifndef SORA_RENDERER_ENV_H_
#define SORA_RENDERER_ENV_H_

#include <string>
#include "core/template_lib.h"
#include "renderer/gl/gl_env.h"

namespace sora {;

enum {
  kRendererNone = 0,
  kRendererGL,
};

template<typename T, int RendererType> class RendererEnvT;
typedef RendererEnvT<GLEnv, kRendererGL> GLRendererEnv;
typedef GLRendererEnv RendererEnv;

//어차피 런타임중에 gl/dx를 분기할 일은 사실상 없고
//컴파일타임에 뭔 환경을 사용할지를 결정할테니까
//템플릿을 써서 해도 문제 없고 구조가 간단할듯하다
template<typename T, int RendererType>
class RendererEnvT {
public:
  enum {
    Type = RendererType,
  };
  //static로 선언한것도 남겨놔서 한방에 접근하는것도 가능하도록하자
public:
  //base info
  static const std::string &Version();
  static const std::string &Vender();
  static const std::string &Renderer();
  static bool IsExtensionSupport(const std::string &ext);
  static const std::vector<std::string> &ExtensionList();

  //error check
  static bool CheckFrameBufferStatus(const std::string &msg);
  static bool CheckError(const std::string &msg);

  //extension data

private:
  static T policy_;
};

//impl
template<typename T, int RendererType>
T RendererEnvT<T, RendererType>::policy_;

template<typename T, int RendererType>
const std::string &RendererEnvT<T, RendererType>::Version() {
  return policy_.GetVersion();
}

template<typename T, int RendererType>
const std::string &RendererEnvT<T, RendererType>::Vender() {
  return policy_.GetVender(); 
}

template<typename T, int RendererType>
const std::string &RendererEnvT<T, RendererType>::Renderer() {
  return policy_.GetRenderer();
}

template<typename T, int RendererType>
bool RendererEnvT<T, RendererType>::IsExtensionSupport(const std::string &ext) {
  using std::vector;
  using std::string;
  const vector<string> &ext_list = env_->GetExtensionList();
  for (size_t i = 0 ; i < ext_list.size() ; i++) {
    if (ext == ext_list[i]) {
      return true;
    }
  }
  return false;
}

template<typename T, int RendererType>
const std::vector<std::string> &RendererEnvT<T, RendererType>::ExtensionList() {
  return policy_.GetExtensionList();
}

template<typename T, int RendererType>
bool RendererEnvT<T, RendererType>::CheckFrameBufferStatus(const std::string &msg) {
  return policy_.CheckFrameBufferStatus(msg);
}

template<typename T, int RendererType>
bool RendererEnvT<T, RendererType>::CheckError(const std::string &msg) {
  return policy_.CheckError(msg);
}

}

//에러검사같은 기능은 매크로로 해놓으면 활성화/비활성화가 쉽다
#define SR_CHECK_ERROR(MSG) RendererEnv::CheckError(MSG);
#define SR_CHECK_FRAMEBUFFER(MSG) RendererEnv::CheckFrameBufferStatus(MSG);

#endif  // SORA_RENDERER_ENV_H_