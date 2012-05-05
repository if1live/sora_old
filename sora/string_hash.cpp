﻿/*  Copyright (C) 2011-2012 by if1live */
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
#include "sora_stdafx.h"
#include "string_hash.h"

namespace sora {;
unsigned int RSHash::Hash(const char *str, int length) {
  unsigned int b    = 378551;
  unsigned int a    = 63689;
  unsigned int hash = 0;

  for(int i = 0; i < length; i++) {
    hash = hash * a + str[i];
    a    = a * b;
  }

  return (hash & 0x7FFFFFFF);
}

unsigned int JSHash::Hash(const char *str, int length) {
  unsigned int hash = 1315423911;

  for(int i = 0; i < length; i++) {
    hash ^= ((hash << 5) + str[i] + (hash >> 2));
  }

  return (hash & 0x7FFFFFFF);
}

unsigned int PJWHash::Hash(const char *str, int length) {
  unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
  unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
  unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
  unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
  unsigned int hash              = 0;
  unsigned int test              = 0;

  for(int i = 0; i < length; i++) {
    hash = (hash << OneEighth) + str[i];
    if((test = hash & HighBits)  != 0) {
      hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
    }
  }

  return (hash & 0x7FFFFFFF);
}

// ELF Hash Function
unsigned int ELFHash::Hash(const char *str, int length) {
  unsigned int hash = 0;
  unsigned int x    = 0;

  for(int i = 0; i < length; i++) {
    hash = (hash << 4) + str[i];
    if((x = hash & 0xF0000000L) != 0) {
      hash ^= (x >> 24);
      hash &= ~x;
    }
  }

  return (hash & 0x7FFFFFFF);
}

// BKDR Hash Function
unsigned int BKDRHash::Hash(const char *str, int length) {
  unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
  unsigned int hash = 0;

  for(int i = 0; i < length; i++) {
    hash = (hash * seed) + str[i];
  }

  return (hash & 0x7FFFFFFF);
}

// SDBM Hash Function
unsigned int SDBMHash::Hash(const char *str, int length) {
  unsigned int hash = 0;
  for(int i = 0; i < length; i++) {
    hash = str[i] + (hash << 6) + (hash << 16) - hash;
  }

  return (hash & 0x7FFFFFFF);
}

// DJB Hash Function
unsigned int DJBHash::Hash(const char *str, int length) {
  unsigned int hash = 5381;
  for(int i = 0; i < length; i++) {
    hash = ((hash << 5) + hash) + str[i];
  }

  return (hash & 0x7FFFFFFF);
}

// DEK Hash Function
unsigned int DEKHash::Hash(const char *str, int length) {
  unsigned int hash = static_cast<unsigned int>(length);

  for(int i = 0; i < length; i++) {
    hash = ((hash << 5) ^ (hash >> 27)) ^ str[i];
  }

  return (hash & 0x7FFFFFFF);
}

// AP Hash Function
unsigned int APHash::Hash(const char *str, int length) {
  unsigned int hash = 0;

  for(int i = 0; i < length; i++) {
    hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ str[i] ^ (hash >> 3)) :
      (~((hash << 11) ^ str[i] ^ (hash >> 5)));
  }

  return (hash & 0x7FFFFFFF);
}

}
