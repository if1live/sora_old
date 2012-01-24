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
#ifndef SORA_INPUT_ENUM_
#define SORA_INPUT_ENUM_

namespace sora {;

typedef enum {
  kTouchBegan,
  kTouchMoved,
  kTouchEnded,
  kTouchCancelled,
} TouchEventType;

// 특수키. glut, glfw에 따라서 이 부분이 달라질수있으니까 따로 enum을 분리함
typedef enum {
  kKeyIdentifierSpace = 256, //이거보다 아래쪽은 기본 아스키로 연결될테니까
  kKeyIdentifierEsc,
  kKeyIdentifierUp,
  kKeyIdentifierDown,
  kKeyIdentifierLeft,
  kKeyIdentifierRight,
} KeyIdentifier;

typedef enum {
  kButtonPressed,
  kButtonReleased
} ButtonState;

}

#endif  // SORA_INPUT_ENUM_