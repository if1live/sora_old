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
#ifndef SORA_SORA_MAIN_H_
#define SORA_SORA_MAIN_H_

#if SR_USE_PCH == 0
#include "core/arch.h"
#endif

SR_C_DLL void SORA_setup_graphics(int w, int h);
SR_C_DLL void SORA_set_window_size(int w, int h);
SR_C_DLL void SORA_cleanup_graphics();
SR_C_DLL void SORA_draw_frame();
SR_C_DLL void SORA_update_frame(float dt);
SR_C_DLL void SORA_init_gl_env();

SR_C_DLL void SORA_set_cam_pos(float a, float b);

#if SR_ANDROID
void SORA_set_apk_file_path(const char *abs_path);
#endif
#endif  // SORA_SORA_MAIN_H_