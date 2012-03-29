// This is the main DLL file.

#include "stdafx.h"

#include "SoraWrapper.h"

#include "sora_main.h"
#include "test_function.h"
#include "sys/device.h"
#include <cstdlib>

namespace SoraWrapper {;
  GLView::GLView() : device_(NULL) {
    device_ = new sora::Device();
  }
  GLView::~GLView() {
    if(device_ != NULL) {
      delete(device_);
      device_ = NULL;
    }
  }
  void GLView::SetupGraphics(int w, int h) {
    SORA_setup_graphics(device_, w, h);
  }
  void GLView::DrawFrame() {
    SORA_draw_frame(device_);
  }
  void GLView::InitGLEnv() {
    SORA_init_gl_env();
  }
  void GLView::UpdateFrame(float dt) {
    SORA_update_frame(device_, dt);
  }
  void GLView::Cleanup() {
    SORA_cleanup_graphics(device_);
  }
  void GLView::SetWindowSize(int w, int h) {
    SORA_set_window_size(device_, w, h);
  }
  void GLView::TestDraw(int w, int h) {
    SORA_test_draw(w, h);
  }
}
