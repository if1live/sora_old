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
#ifndef BASE_SORA_STRING_HASH_FUNCTOR_H_
#define BASE_SORA_STRING_HASH_FUNCTOR_H_

#include <string>

///http://blog.daum.net/aswip/4538253
namespace sora {;

struct RSHash;
struct JSHash;
struct PJWHash;
struct ELFHash;
struct BKDRHash;
struct SDBMHash;
struct DJBHash;
struct DEKHash;
struct APHash;

struct RSHash {
  // RS Hash Function
  unsigned int operator()(const std::string& str) const;
};

struct JSHash {
  // JS Hash Function
  unsigned int operator()(const std::string& str) const;
};

struct PJWHash {
  // P. J. Weinberger Hash Function
  unsigned int operator()(const std::string& str) const;
};

struct ELFHash {
  // ELF Hash Function
  unsigned int operator()(const std::string& str) const;
};

struct BKDRHash {
  // BKDR Hash Function
  unsigned int operator()(const std::string& str) const;
};

struct SDBMHash {
  // SDBM Hash Function
  unsigned int operator()(const std::string& str) const;
};

struct DJBHash {
  // DJB Hash Function
  unsigned int operator()(const std::string& str) const;
};

struct DEKHash {
  // DEK Hash Function
  unsigned int operator()(const std::string& str) const;
};

struct APHash {
  // AP Hash Function
  unsigned int operator()(const std::string& str) const;
};

}

#endif  // BASE_SORA_STRING_HASH_FUNCTOR_H_
