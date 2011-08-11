// Ŭnicode please
#include "../SoraStdAfx.h"
#include "chizuru/CHLocale.h"

namespace chizuru
{
	///시스템 기본 로케일은 etc+etc이다. 적절히 필요한 국가에 맞춰서 설정하자
	///이것은 직접 시스템에서 로케일을 읽는다기보다는 유저정의 로케일을 저장하는 목적이 강하다
	Locale::Locale()
		: country_(CountryTypeETC), lang_(LanguageTypeETC)
	{
	}
	Locale::~Locale()
	{
	}
	CountryType Locale::getCountry() const
	{
		return country_;
	}
	LanguageType Locale::getLanguage() const
	{
		return lang_;
	}
	void Locale::setCountry(CountryType country)
	{
		country_ = country;
	}
	void Locale::setLanguage(LanguageType lang)
	{
		lang_ = lang;
	}

	void Locale::setKorea()
	{
		country_ = CountryTypeKorea;
		lang_ = LanguageTypeKorean;
	}
	void Locale::setUSA()
	{
		country_ = CountryTypeUSA;
		lang_ = LanguageTypeEnglish;
	}
	void Locale::setETC()
	{
		country_ = CountryTypeETC;
		lang_ = LanguageTypeETC;
	}
	Locale &Locale::sharedLocale()
	{
		static Locale l;
		return l;
	}
}
