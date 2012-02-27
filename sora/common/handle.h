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
#ifndef SORA_HANDLE_H_
#define SORA_HANDLE_H_

namespace sora {;
template<typename TagType>  class Handle;

template<typename TagType>
class Handle {
public:
  enum {
    kMaxBitsIndex = 16,
    kMaxBitsMagic = 16,
    kMaxIndex = (1 << kMaxBitsIndex) - 1,
    kMaxMagic = (1 << kMaxBitsMagic) - 1,
  };
public:
  Handle() : handle_(0) { }
  Handle(int index) : handle_(0) { Init(index); }
  // 원하는 핸들을 강제로 만들기. 수동으로 뭔가를 얻어서 그 정보를 다시 핸들로 써야될떄 쓴다
  // 나중에는 타입안정성을 박살내는 원인이 될지도?
  Handle(u16 index, u16 magic) : res_index_(index), res_magic_(magic) {}
  ~Handle() { }
 
  u32 res_index() const { return res_index_; }
  u32 res_magic() const { return res_magic_; }
  void set_res_index(int i) { res_index_ = i; }
  u32 handle() const { return handle_; }
  bool IsNull() const { return (!handle_); }
  operator u32() const { return handle_; }

  void Init(u32 index) {
    SR_ASSERT(IsNull());    // 이미 사용되는 핸들 고치기 불가능
    SR_ASSERT(index <= kMaxIndex);

    static unsigned int auto_magic = 0; // magic=0 == null handle
    auto_magic++;
    if (auto_magic > kMaxMagic) {
      auto_magic = 1;
    }
    res_index_ = index;
    res_magic_ = auto_magic;
  }

  bool operator!=(const Handle<TagType> &o) const {
    return !(*this == o);
  }
  bool operator==(const Handle<TagType> &o) const {
    return (this->handle() == o.handle());
  }

private:
  union {
    struct {
      unsigned res_index_ : kMaxBitsIndex;  // 자원 벡터에 대한 인덱스
      unsigned res_magic_ : kMaxBitsMagic;  // 오류 확인용
    };
    u32 handle_;
  };
};


}

#endif