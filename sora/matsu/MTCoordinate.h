// Ŭnicode please
#pragma once

#include "matsu/MTMacro.h"

namespace matsu
{
	template<typename T>
	class SphericalPoint;
	template<typename T>
	class Rectangular3Point;
	
	template<typename T>
	class SphericalPoint {
	public:
		static SphericalPoint createRadianBase(T radius, T thetaRadian, T piRadian);
		static SphericalPoint createDegreeBase(T radius, T thetaDegree, T piDegree);
		static SphericalPoint create(const Rectangular3Point<T> &o);
		
		//getter
		T getRadius() const;
		T getThetaDegree() const;
		T getPiDegree() const;
		T getThetaRadian() const;
		T getPiRadian() const;
		
		//setter
		void setRadius(T r);
		void setThetaDegree(T deg);
		void setThetaRadian(T rad);
		void setPiDegree(T deg);
		void setPiRadian(T rad);
		
	private:
		///아래의 값은 degree로 저장한다.
		///c std math함수의 인자는 전부다 rad기준이지만 어차피 deg-rad변환은 간단하니까
		///속도상 큰 문제가 없을것이라고 추정되서 읽기 쉬운 deg를 기준으로 했다.
		///rad가 0.43rad같은게 뜨면 감이 전혀 안오니까 이쪽이 개발에는 더 나을것이라고 판단했다
		///후에 rad-deg의 변환을 바꾸는게 좋을거같으면 외부인터페스는 적당히 두고 여기를 고쳐서 떄우자 
		T radius_;
		T thetaDegree_;
		T piDegree_;
	};
	
	template<typename T>
	class Rectangular3Point {
	public:
		Rectangular3Point(T x, T y, T z);
		static Rectangular3Point create(const SphericalPoint<T> &o);
		
		T getX() const;
		T getY() const;
		T getZ() const;
	private:
		T x_;
		T y_;
		T z_;
	};
	
	template<typename T>
	class SphericalCoordinateSystem {
	public:
	};
	
	template<typename T>
	class Rectangular3CoordinateSystem {
	public:
		Rectangular3Point<T> convert(const SphericalPoint<T> &target);
	};
}

//impl
#include "matsu/MTCoordinate.hpp"