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
#include "arch.h"
#endif

namespace sora {;
class Device;
}

//뭔 main을 갖다쓰더라도 공통되는 부분
SR_C_DLL void SORA_set_window_size(sora::Device *device, int w, int h);
SR_C_DLL void SORA_init_gl_env();
#if SR_ANDROID
void SORA_set_apk_file_path(const char *abs_path);
#endif

//구현 의존적인 부분
SR_C_DLL void SORA_setup_graphics(sora::Device *device, int w, int h);
SR_C_DLL void SORA_cleanup_graphics(sora::Device *device);
SR_C_DLL void SORA_draw_frame(sora::Device *device);
SR_C_DLL void SORA_update_frame(sora::Device *device, float dt);

SR_C_DLL void SORA_set_cam_pos(float a, float b);

SR_C_DLL void SORA_touch_began(sora::Device *device, int x, int y, int uid, int tick_count, float timestamp);
SR_C_DLL void SORA_touch_moved(sora::Device *device, int x, int y, int prev_x, int prev_y, int uid, int tick_count, float timestamp);
SR_C_DLL void SORA_touch_ended(sora::Device *device, int x, int y, int prev_x, int prev_y, int uid, int tick_count, float timestamp);
SR_C_DLL void SORA_touch_cancelled(sora::Device *device, int x, int y, int prev_x, int prev_y, int uid, int tick_count, float timestamp);


#endif  // SORA_SORA_MAIN_H_