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
#ifndef SORA_STRING_HASH_FUNCTOR_H_
#define SORA_STRING_HASH_FUNCTOR_H_

#include <string>

///http://blog.daum.net/aswip/4538253
namespace sora {;
struct RSHash {
  // RS Hash Function
  unsigned int operator()(const std::string& str) const {
    unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;

    for(std::size_t i = 0; i < str.length(); i++) {
      hash = hash * a + str[i];
      a = a * b;
    }
    return (hash & 0x7FFFFFFF);
  }
};

struct JSHash {
  // JS Hash Function
  unsigned int operator()(const std::string& str) const {
    unsigned int hash = 1315423911;
    for(std::size_t i = 0; i < str.length(); i++) {
      hash ^= ((hash << 5) + str[i] + (hash >> 2));
    }
    return (hash & 0x7FFFFFFF);
  }
};

struct PJWHash {
  // P. J. Weinberger Hash Function
  unsigned int operator()(const std::string& str) const {
    unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
    unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
    unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    unsigned int hash              = 0;
    unsigned int test              = 0;

    for(std::size_t i = 0; i < str.length(); i++) {
      hash = (hash << OneEighth) + str[i];
      if((test = hash & HighBits)  != 0) {
        hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
      }
    }
    return (hash & 0x7FFFFFFF);
  }
};

struct ELFHash {
  // ELF Hash Function
  unsigned int operator()(const std::string& str) const {
    unsigned int hash = 0;
    unsigned int x    = 0;
    for(std::size_t i = 0; i < str.length(); i++) {
      hash = (hash << 4) + str[i];
      if((x = hash & 0xF0000000L) != 0) {
        hash ^= (x >> 24);
        hash &= ~x;
      }
    }
    return (hash & 0x7FFFFFFF);
  }
};

struct BKDRHash {
  // BKDR Hash Function
  unsigned int operator()(const std::string& str) const {
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    for(std::size_t i = 0; i < str.length(); i++) {
      hash = (hash * seed) + str[i];
    }
    return (hash & 0x7FFFFFFF);
  }
};

struct SDBMHash {
  // SDBM Hash Function
  unsigned int operator()(const std::string& str) const {
    unsigned int hash = 0;
    for(std::size_t i = 0; i < str.length(); i++) {
      hash = str[i] + (hash << 6) + (hash << 16) - hash;
    }
    return (hash & 0x7FFFFFFF);
  }
};

struct DJBHash {
  // DJB Hash Function
  unsigned int operator()(const std::string& str) const {
    unsigned int hash = 5381;
    for(std::size_t i = 0; i < str.length(); i++) {
      hash = ((hash << 5) + hash) + str[i];
    }
    return (hash & 0x7FFFFFFF);
  }
};

struct DEKHash {
  // DEK Hash Function
  unsigned int operator()(const std::string& str) const {
    unsigned int hash = static_cast<unsigned int>(str.length());
    for(std::size_t i = 0; i < str.length(); i++) {
      hash = ((hash << 5) ^ (hash >> 27)) ^ str[i];
    }
    return (hash & 0x7FFFFFFF);
  }
};

struct APHash {
  // AP Hash Function
  unsigned int operator()(const std::string& str) const {
    unsigned int hash = 0;
    for(std::size_t i = 0; i < str.length(); i++) {
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ str[i] ^ (hash >> 3)) :
        (~((hash << 11) ^ str[i] ^ (hash >> 5)));
    }
    return (hash & 0x7FFFFFFF);
  }
};
}

#endif  // SORA_STRING_HASH_FUNCTOR_H_
