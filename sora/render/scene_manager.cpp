﻿/*  Copyright (C) 2011 by if1live */
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
#include "scene_manager.h"
#include "sora/template_library.h"
#include "scene.h"

#if SR_USE_PCH == 0
#include <algorithm>
#endif

namespace sora {;
SceneManager::SceneManager() {
}
SceneManager::~SceneManager() {
  DestroyList(&delete_stack_);
  DestroyList(&scene_stack_);
}
Scene *SceneManager::Top() {
  if (scene_stack_.empty()) {
    return NULL;
  } else {
    return scene_stack_.back();
  }
}
void *SceneManager::Push(Scene *scene) {
  SR_ASSERT(scene != NULL);
  scene_stack_.push_back(scene);
}

//최상위씬을 얻고 그것을 스택에서 빼낸다
Scene *SceneManager::Pop() {
  if (scene_stack_.empty()) {
    return NULL;
  } else {
    Scene *scene = scene_stack_.back();
    scene_stack_.pop_back();
  }
}

//최상위 씬을 파기+꺼내기
void SceneManager::PopAndDestroy() {
  if (!scene_stack_.empty()) {
    Scene *scene = scene_stack_.back();
    scene_stack_.pop_back();

    RequestDelete(scene);
  }
}

bool SceneManager::IsEmpty() const {
  return scene_stack_.empty();
}
void SceneManager::RequestDelete(Scene *scene) {
  SceneStackType::iterator found = find(delete_stack_.begin(),
    delete_stack_.end(), scene);
  if (found == delete_stack_.end()) {
    delete_stack_.push_back(scene);
  }
}
void SceneManager::Update(int dt_ms) {
  Scene *top = Top();
  if (top != NULL) {
    top->Update(dt_ms);
  }
  DestroyList(&delete_stack_);
}
void SceneManager::Draw() {
  Scene *top = Top();
  if (top != NULL) {
    top->Draw();
  }
}
}