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
#include "sora_stdafx.h"
#include "sys_locale.h"

namespace sora {;

int locale_table[][2] = {
  { kCountryKorea, kLanguageKorean },
  { kCountryUSA, kLanguageEnglish },
  { kCountryETC, kLanguageETC }
};

///시스템 기본 로케일은 etc+etc이다. 적절히 필요한 국가에 맞춰서 설정하자
///이것은 직접 시스템에서 로케일을 읽는다기보다는 유저정의 로케일을 저장하는 목적이 강하다
Locale::Locale()
  : country_(kCountryETC), lang_(kLanguageETC) {
}
Locale::~Locale() {
}

void Locale::SetLocale(CountryType country) {
  for (int i = 0 ; i < kCountryCount ; i++) {
    if (locale_table[i][0] == country) {
      country_ = (CountryType)locale_table[i][0];
      lang_ = (LanguageType)locale_table[i][1];
      return;
    }
  }
  //else...
  country_ = kCountryETC;
  lang_ = kLanguageETC;
}
}
