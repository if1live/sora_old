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
#ifndef SORA_STRING_HASH_H_
#define SORA_STRING_HASH_H_

#include <string>

//http://blog.daum.net/aswip/4538253
//문자열 해쉬함수 모음 (String Hash Function) 
namespace sora {;
struct RSHash {
	static unsigned int Hash(const std::string &str) {
    return Hash(str.data(), str.length());
  }
  static unsigned int Hash(const char *str, int length);
};

struct JSHash {
	static unsigned int Hash(const std::string &str) {
    return Hash(str.data(), str.length());
  }
  static unsigned int Hash(const char *str, int length);
};

// P. J. Weinberger Hash Function
struct PJWHash {
	static unsigned int Hash(const std::string &str) {
    return Hash(str.data(), str.length());
  }
  static unsigned int Hash(const char *str, int length);
};

struct ELFHash {
	static unsigned int Hash(const std::string &str) {
    return Hash(str.data(), str.length());
  }
  static unsigned int Hash(const char *str, int length);
};


struct BKDRHash {
	static unsigned int Hash(const std::string &str) {
    return Hash(str.data(), str.length());
  }
  static unsigned int Hash(const char *str, int length);
};

struct SDBMHash {
	static unsigned int Hash(const std::string &str) {
    return Hash(str.data(), str.length());
  }
  static unsigned int Hash(const char *str, int length);
};

struct DJBHash {
	static unsigned int Hash(const std::string &str) {
    return Hash(str.data(), str.length());
  }
  static unsigned int Hash(const char *str, int length);
};

struct DEKHash {
	static unsigned int Hash(const std::string &str) {
    return Hash(str.data(), str.length());
  }
  static unsigned int Hash(const char *str, int length);
};

struct APHash {
	static unsigned int Hash(const std::string &str) {
    return Hash(str.data(), str.length());
  }
  static unsigned int Hash(const char *str, int length);
};
}

#endif  // SORA_STRING_HASH_H_