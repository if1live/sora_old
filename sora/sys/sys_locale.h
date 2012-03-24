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
#ifndef SORA_LOCALE_H_
#define SORA_LOCALE_H_

//#include "core/template_lib.h"

namespace sora {;
typedef enum {
  kCountryKorea,
  kCountryUSA,
  kCountryETC,
  kCountryCount,
} CountryType;

typedef enum {
  kLanguageKorean,
  kLanguageEnglish,
  kLanguageETC,
} LanguageType;

class SR_DLL Locale {
public:
  Locale();
  ~Locale();

  //dll과 singleton tpl이 같이 안돌아가서 임시로 이렇게 처리함
  static Locale& GetInstance() {
    static Locale ctx;
    return ctx;
  }

  CountryType country() const { return country_; }
  LanguageType language() const { return lang_; }
  void set_country(CountryType country) { country_ = country; }
  void set_language(LanguageType lang) { lang_ = lang; }

  //너무 뻔한 국가설정은 바로할수 있도록했음
  void SetKorea() { SetLocale(kCountryKorea); }
  void SetUSA() { SetLocale(kCountryUSA); }
  void SetETC() { SetLocale(kCountryETC); }

  //locale, 국가 동시에 변경
  void SetLocale(CountryType country);

private:
  CountryType country_;
  LanguageType lang_;
};
}
#endif