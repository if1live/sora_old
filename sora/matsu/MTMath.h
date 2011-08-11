// Ŭnicode please
#pragma once

#include <cmath>
#include "matsu/MTMacro.h"

namespace matsu
{
	template<typename T>
	bool equalAbsError(T a, T b, T maxError);
	
	template<typename T>
	bool equalRelError(T a, T b, T maxError);
	
	template<typename T>
	bool equalUlps(T a, T b, int maxUlps);
	
	template<typename T>
	T deg2rad(T deg);

	template<typename T>
	T rad2deg(T rad);
	
	template<typename T>
	T degree2radian(T degree);
	
	template<typename T>
	T radian2degree(T radian);
	
	///@brief target이 base의 지수승으로 표현가능한 숫자인가
	template<typename T>
	bool isPower(T base, T target);
	
	///@brief target보다 크거가 같은 base의 지수승으로 표현 가능한 숫자
	template<typename T>
	T ceilPower(T base, T target);

	template<typename T>
	bool isNaN(T num);

	template<typename T>
	bool isInf(T num);

	///@brief nan이나 무한대가 아닌수이면 true
	template<typename T>
	bool isNormalNumber(T num);

	///@brief atoi, atof를 할때 스트링클래스를 직접 못넣고 c_str을 거쳐가는것이
	///매우 귀찮아서 만든 함수. 템플릿으로 만들어서 캐스팅 한번더 할필요 없이
	///원하는 타입을 얻을수 있도록 했다
	template<typename T>
	T str2int(const std::string &str);

	template<typename T>
	T str2float(const std::string &str);
	
	template<typename T>
	T calcIncludeAngleDegree(T src, T dst);
	
	template<typename T>
	T calcIncludeAngleRadian(T src, T dst);
	
	//삼각함수 wrapper
	template<typename T>
	T sinRad(T rad);
	template<typename T>
	T sinDeg(T deg);
	
	template<typename T>
	T cosRad(T rad);
	template<typename T>
	T cosDeg(T deg);
	
	template<typename T>
	T tanRad(T rad);
	template<typename T>
	T tanDeg(T deg);
	
	///@brief asin, acos의 경우는 입력이 -1~1이어야 정상적으로 작동한다
	///정상범위인지 확인하기 위해서 오차를 받아서 대강 그 근처면 보정하고
	///그것이 아니면 assert로 죽자
	template<typename T>
	T asinRad(T value, T maxError = 0.00001);
	template<typename T>
	T asinDeg(T value, T maxError = 0.00001);
	
	template<typename T>
	T acosRad(T value, T maxError = 0.00001);
	template<typename T>
	T acosDeg(T value, T maxError = 0.00001);
	
	template<typename T>
	T atanRad(T value);
	template<typename T>
	T atanDeg(T value);
	
	template<typename T>
	T lerp(T from, T to, float t);
}

#include "matsu/MTMath.hpp"
