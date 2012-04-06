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
#include "Stdafx.h"
#include "sora_view.h"
#include "sora_main.h"
#include "test_function.h"
#include "sys/device.h"
#include <cstdlib>


namespace sora {;
  SoraView::SoraView() {
  }
  SoraView::~SoraView() {
  }
  void SoraView::SetupGraphics(int w, int h) {
    Device *dev = &device();
    SORA_setup_graphics(dev, w, h);
  }
  void SoraView::DrawFrame() {
    Device *dev = &device();
    SORA_draw_frame(dev);
  }
  void SoraView::UpdateFrame(float dt) {
    Device *dev = &device();
    SORA_update_frame(dev, dt);
  }
  void SoraView::Cleanup() {
    Device *dev = &device();
    SORA_cleanup_graphics(dev);
  }
  void SoraView::SetWindowSize(int w, int h) {
    Device *dev = &device();
    SORA_set_window_size(dev, w, h);
  }
  void SoraView::TestDraw(int w, int h) {
    //SORA_test_draw(w, h);
  }
}
