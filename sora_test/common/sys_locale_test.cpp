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
#include "sora_test_stdafx.h"
#include "sora/common/sys_locale.h"

TEST(Locale, static_set_country) {
  using namespace sora;
	Locale locale;
	locale.SetKorea();
	EXPECT_EQ(kCountryKorea, locale.country());
	EXPECT_EQ(kLanguageKorean, locale.language());

	locale.SetUSA();
	EXPECT_EQ(kCountryUSA, locale.country());
	EXPECT_EQ(kLanguageEnglish, locale.language());

	locale.SetETC();
	EXPECT_EQ(kCountryETC, locale.country());
	EXPECT_EQ(kLanguageETC, locale.language());
}

TEST(Locale, Setter_Getter) {
  using namespace sora;
	Locale locale;

	locale.set_country(kCountryKorea);
	locale.set_language(kLanguageEnglish);
	EXPECT_EQ(kCountryKorea, locale.country());
	EXPECT_EQ(kLanguageEnglish, locale.language());
}