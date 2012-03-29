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
#include "sora_stdafx.h"
#include "render_state.h"

#include "sys/device.h"
#include "renderer/gl_helper.h"

namespace sora {;
RenderState::RenderState(Device *dev) {
  memset(this, 0, sizeof(RenderState));
  dev_ = dev;
}
RenderState::~RenderState() {
}
void RenderState::SetWinSize(int w, int h) {
  if(win_width_ != w || win_height_ != h) {
    LOGI("setupGraphics(%d, %d)", (int)w, (int)h);
    win_width_ = w;
    win_height_ = h;

    glViewport(0, 0, (int)w, (int)h);
    GLHelper::CheckError("glViewport");
  }
}
}