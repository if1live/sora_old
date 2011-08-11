// Ŭnicode please
#include "../SoraTestStdAfx.h"
#include <chizuru/CHLocale.h>

using namespace chizuru;

TEST(CHLocale, static_setCountry)
{
	Locale locale;
	locale.setKorea();
	EXPECT_EQ(CountryTypeKorea, locale.getCountry());
	EXPECT_EQ(LanguageTypeKorean, locale.getLanguage());

	locale.setUSA();
	EXPECT_EQ(CountryTypeUSA, locale.getCountry());
	EXPECT_EQ(LanguageTypeEnglish, locale.getLanguage());

	locale.setETC();
	EXPECT_EQ(CountryTypeETC, locale.getCountry());
	EXPECT_EQ(LanguageTypeETC, locale.getLanguage());
}

TEST(CHLocale, Setter_Getter)
{
	Locale locale;

	locale.setCountry(CountryTypeKorea);
	locale.setLanguage(LanguageTypeEnglish);
	EXPECT_EQ(CountryTypeKorea, locale.getCountry());
	EXPECT_EQ(LanguageTypeEnglish, locale.getLanguage());
}