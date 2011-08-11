// Ŭnicode please
#pragma once

#include "matsu/MTSingleton.h"

namespace chizuru
{
	typedef enum {
		CountryTypeKorea,
		CountryTypeUSA,
		CountryTypeETC
	} CountryType;

	typedef enum {
		LanguageTypeKorean,
		LanguageTypeEnglish,
		LanguageTypeETC,
	} LanguageType;

	class Locale {
	public:
		Locale();
		~Locale();
		
		CountryType getCountry() const;
		LanguageType getLanguage() const;
		void setCountry(CountryType country);
		void setLanguage(LanguageType lang);

		//너무 뻔한 국가설정은 바로할수 있도록했음
		void setKorea();
		void setUSA();
		void setETC();

		static Locale &sharedLocale();
	private:
		CountryType country_;
		LanguageType lang_;
	};
}