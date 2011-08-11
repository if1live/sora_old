// Ŭnicode please
#include "matsu/MTVector.h"

namespace matsu
{
	template<typename T>
	bool equalAbsError(T a, T b, T maxError)
	{
		if(a == b)
		{
			return true;
		}
		if(abs(a - b) <= maxError)
		{
			return true;
		}
		//else..
		return false;
	}
	
	template<typename T>
	bool equalRelError(T a, T b, T maxError)
	{
		if(fabs(a - b) < maxError)
		{
			return true;
		}
		T relativeError = 0;
		if(abs(b) > abs(a))
		{
			relativeError = abs((a-b) / b);
		}
		else
		{
			relativeError = abs((a-b) / a);
		}
		if(relativeError <= maxError)
		{
			return true;
		}
		//else..
		return false;
	}
	
	template<typename T>
	bool equalUlps(T a, T b, int maxUlps)
	{
		//http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
		MT_ASSERT(sizeof(float) == sizeof(int));
		if (a == b)
		{
			return true;
		}
		int intDiff = abs(*(int*)&a - *(int*)&b);
		if (intDiff <= maxUlps)
		{
			return true;
		}
		return false;
	}
	
	template<typename T>
	T deg2rad(T deg)
	{
		return degree2radian(deg);
	}
	
	template<typename T>
	T rad2deg(T rad)
	{
		return radian2degree(rad);
	}
	
	template<typename T>
	T degree2radian(T degree)
	{
		return degree * MT_PI / 180;
	}
	template<typename T>
	T radian2degree(T radian)
	{
		return radian * 180 / MT_PI;
	}
	
	template<typename T>
	bool isPower(T base, T target)
	{
		if(target < 1)
		{
			//2*-1=0.5인데 이런 경우는 쓰지 않을듯해서 신경쓰지 않는다
			//어차피 위와같이 분수로 내려가는 경우, 정밀도 문제로
			//쓰지도 못할거같다
			return false;
		}
		T curr = base;
		while(curr <= target)
		{
			if(curr == target)
			{
				return true;
			}
			curr *= base;
		}
		return false;
	}
	
	template<typename T>
	T ceilPower(T base, T target)
	{
		if(target < 1)
		{
			//소수점이하는 계산무시
			return base;
		}
		T curr = base;
		while(curr < target)
		{
			curr *= base;
		}
		return curr;
	}

	template<typename T>
	bool isNaN(T num)
	{
		return (num != num);
	}

	template<typename T>
	bool isInf(T num)
	{
		if(std::numeric_limits<T>::has_infinity == false)
		{
			return false;
		}
		if(num == std::numeric_limits<T>::infinity())
		{
			return true;
		}
		//else..
		return false;
	}

	template<typename T>
	bool isNormalNumber(T num)
	{
		if(isNaN(num) || isInf(num))
		{
			return false;
		}
		//else...
		return true;
	}

	template<typename T>
	T str2int(const std::string &str)
	{
		int result = atoi(str.c_str());
		return static_cast<T>(result);
	}
	
	template<typename T>
	T str2float(const std::string &str)
	{
		//double이 큰 형태이고 원래 atof의 타입이 double더라
		double result = atof(str.c_str());
		return static_cast<T>(result);
	}
	
	template<typename T>
	T calcIncludeAngleDegree(T src, T dst)
	{
		T radResult = calcIncludeAngleRadian(deg2rad(src), deg2rad(dst));
		return rad2deg(radResult);
	}
	
	template<typename T>
	T calcIncludeAngleRadian(T src, T dst)
	{
		vec2 srcVec2(cos(src), sin(src));
		vec2 dstVec2(cos(dst), sin(dst));
		float vecDot = srcVec2.dot(dstVec2);
		float srcSize = srcVec2.size();
		float dstSize = dstVec2.size();
		float value = vecDot / srcSize / dstSize;
		if(value >= 1.0f) {
			value = 1.0f;
		} else if(value <= -1.0f) {
			value = -1.0f;
		}
		float rawAbsRadian = acos(value);
		MT_ASSERT(isNaN(rawAbsRadian) == false);
		
		//벡터의 외적을 이용해서 zero-curr이 양수방향인지 음수방향인지를 결정한다
		vec3 srcVec3(srcVec2);
		vec3 dstVec3(dstVec2);
		vec3 crossPan = srcVec3.cross(dstVec3);
		if(crossPan.getZ() > 0) {
			return rawAbsRadian;
		} else {
			return -rawAbsRadian;
		}
	}
	
	template<typename T>
	T sinRad(T rad)
	{
		return sin(rad);
	}
	template<typename T>
	T sinDeg(T deg)
	{
		return sinRad(deg2rad(deg));
	}
	
	template<typename T>
	T cosRad(T rad)
	{
		return cos(rad);
	}
	template<typename T>
	T cosDeg(T deg)
	{
		return cosRad(deg2rad(deg));
	}
	
	template<typename T>
	T tanRad(T rad)
	{
		return tan(rad);
	}
	template<typename T>
	T tanDeg(T deg)
	{
		return tanRad(deg2rad(deg));
	}
	
	template<typename T>
	T asinRad(T value, T maxError)
	{
		T result = 0;
		if(value >= static_cast<T> (1.0) && value <= static_cast<T> (1.0 + maxError)) 
		{
			value = static_cast<T> (1.0);
		}
		else if(value <= static_cast<T> (-1.0) && value >= static_cast<T>(-1.0 + maxError))
		{
			value = static_cast<T> (-1.0);
		}
		else if(value >= static_cast<T> (-1.0) && value <= static_cast<T> (1))
		{
			; //정상 범위
		}
		else
		{
			MT_ASSERT(!"not valid value, check range -1~1");
		}
		return asin(value);

	}
	template<typename T>
	T asinDeg(T value, T maxError)
	{
		return asinRad(deg2rad(value, maxError));
	}
	
	template<typename T>
	T acosRad(T value, T maxError)
	{
		T result = 0;
		if(value >= static_cast<T> (1.0) && value <= static_cast<T> (1.0 + maxError)) 
		{
			value = static_cast<T> (1.0);
		}
		else if(value <= static_cast<T> (-1.0) && value >= static_cast<T>(-1.0 + maxError))
		{
			value = static_cast<T> (-1.0);
		}
		else if(value >= static_cast<T> (-1.0) && value <= static_cast<T> (1))
		{
			; //정상 범위
		}
		else
		{
			MT_ASSERT(!"not valid value, check range -1~1");
		}
		return acos(value);
	}
	template<typename T>
	T acosDeg(T value, T maxError)
	{
		return acosDeg(deg2rad(value, maxError));
	}
	
	template<typename T>
	T atanRad(T value)
	{
		return atan(value);
	}
	template<typename T>
	T atanDeg(T value)
	{
		return atanRad(deg2rad(value));
	}
	
	template<typename T>
	T lerp(T from, T to, float t)
	{
		return from * (1-t) + to * t;
	}
}