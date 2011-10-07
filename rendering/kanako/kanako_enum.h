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
#ifndef RENDERING_KANAKO_KANAKO_ENUM_H_
#define RENDERING_KANAKO_KANAKO_ENUM_H_

namespace kanako {;
class Font;
class Label;
class TextArea;

// vincent 폰트를 기반으로 텍스쳐 만들기
// 128글자, 8*8폰트니까
// 256글자를 저장할수있는 영역을 할당하고
// 16 * 16개의 글자를 배열할수있도록 텍스쳐의 크기를
// 128 * 128의 텍스쳐 영역에 글자를 적절히 배치한다
const int kTextureWidth = 128;
const int kTextureHeight = 128;
const int kFontSize = 8;
}

#endif  // RENDERING_KANAKO_KANAKO_ENUM_H_
