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
#ifndef SORA_SCENE_MANAGER_H_
#define SORA_SCENE_MANAGER_H_

#include "sora/template_library.h"

#if SR_USE_PCH == 0
#include <vector>
#endif

namespace sora {;

class Scene;

class SceneManager : public Singleton<SceneManager> {
public:
  SceneManager();
  
  Scene *Top();
  void *Push(Scene *scene);

  //최상위씬을 얻고 그것을 스택에서 빼낸다
  Scene *Pop();

  //최상위 씬을 파기+꺼내기
  void PopAndDestroy();

  bool IsEmpty() const;
  void RequestDelete(Scene *scene);

  void Update(int dt_ms);
  void Draw();
private:
  ~SceneManager();
  
  typedef std::vector<Scene*> SceneStackType;
  SceneStackType scene_stack_;

  // 씬의 삭제를 바로 처리할 경우, 돌아가던 로직이 뻑날수 있으니
  // 삭제를 요청했다가 나중에 지우는것을 지원하도록하자
  SceneStackType delete_stack_;
};
}

#endif  // SORA_SCENE_MANAGER_H_