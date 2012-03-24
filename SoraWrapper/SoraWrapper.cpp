// This is the main DLL file.

#include "stdafx.h"

#include "SoraWrapper.h"

#include "sora_main.h"

namespace SoraWrapper {
  void GLView::SetupGraphics(int w, int h)
  {
    SORA_setup_graphics(w, h);
  }
  void GLView::DrawFrame()
  {
    SORA_draw_frame();
  }
  void GLView::InitGLEnv()
  {
    SORA_init_gl_env();
  }
  void GLView::UpdateFrame(float dt)
  {
    SORA_update_frame(dt);
  }
  void GLView::Cleanup()
  {
    SORA_cleanup_graphics();
  }
  void GLView::SetWindowSize(int w, int h)
  {
    SORA_set_window_size(w, h);
  }
}
