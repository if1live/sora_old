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
#ifndef SORA_TEXTURE_H_
#define SORA_TEXTURE_H_

namespace sora {;

class Texture {
public:
  enum {
    kFileUnknown = -1,
    kFilePNG,
  };
  enum {
    kPolicyForcePOT = 0x01, //2의 승수로 강제 보정할것인가
  };

public:
  Texture(uint policy = 0);
  ~Texture();

  void Deinit();
  void SetData(int file_fmt, uchar *start, uchar *end);
  bool Init();
  //외부에서 생성된 GL텍스쳐를 sora텍스쳐로 사용하기
  bool Init(uint tex_id, int width, int height);
  bool Loaded() const;

  uint handle() const { return handle_; }

private:
  uint handle_;
  //텍스쳐 정보가 잇는 메모리를 올려서
  //텍스쳐를 나중에 생성하는것이 가능하도록하자
  int file_fmt_;
  uchar *start_;
  uchar *end_;

  int tex_width_;
  int tex_height_;
  int src_width_;
  int src_height_;

  uint policy_;

private:
  bool Init_PNG();  //파일 포맷별 로딩을 다르게 할수잇도록. 함수깊이를 얕게 하기 위해서
};

}

#endif  // SORA_TEXTURE_H_