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
#ifndef SORA_GL_WINDOW_H_
#define SORA_GL_WINDOW_H_

namespace sora {;

typedef enum {
  kWinModeWindow,
  kWinModeFullscreen
} WinMode;

class GLWindow {
public:
  GLWindow(int w, int h, WinMode mode, float content_scale);
  ~GLWindow();

  void Init();

  int width() const { return width_; }
  int height() const { return height_; }
  int SetSize(int w, int h) { width_ = w; height_ = h; }
  float content_scale() const { return content_scale_; }
  void set_conetent_scale(float s) { content_scale_ = s; }

private:
  int width_;
  int height_;
  float content_scale_;
  WinMode win_mode_;
};
}

#endif  // SORA_GL_WINDOW_
