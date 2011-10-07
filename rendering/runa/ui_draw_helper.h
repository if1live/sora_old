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
#ifndef RENDERING_RUNA_DRAW_UI_HELPER_H_
#define RENDERING_RUNA_DRAW_UI_HELPER_H_

#include "matsu/matsu_enum.h"

namespace runa {;
/// @brief ui좌표계에서 뭔가를 그리는 함수 모음
class UIDrawHelper {
public:
  static void DrawLine(const matsu::vec2 &p1,
    const matsu::vec2 &p2, const matsu::vec4 &color);
  static void DrawLine(const std::vector<matsu::vec2> &ui_point_list,
    const matsu::vec4 &color);
  /// @brief rect는 원점이 왼쪽위이다. UI좌표계와 맞춤
  static void DrawLineRect(const matsu::Rectf &rect, const matsu::vec4 &color);
  static void DrawRect(const matsu::Rectf &rect, const matsu::vec4 &color);
};
}

#endif  // RENDERING_RUNA_DRAW_UI_HELPER_H_
