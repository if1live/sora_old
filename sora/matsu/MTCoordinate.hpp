// Ŭnicode please
#include "matsu/MTMath.h"

namespace matsu
{
	
	template<typename T>
	SphericalPoint<T> SphericalPoint<T>::createRadianBase(T radius, T thetaRadian, T piRadian)
	{
		T theraDegree = radian2degree(thetaRadian);
		T piDegree = radian2degree(piRadian);
		return createDegreeBase(radius, theraDegree, piDegree);
	}
	
	template<typename T>
	SphericalPoint<T> SphericalPoint<T>::createDegreeBase(T radius, T thetaDegree, T piDegree)
	{
		SphericalPoint p;
		//radius, theta, pi의 값이 적절한 범위에 존재하는지 확인하고 아니면 적절한값으로 변환 
		p.setRadius(radius);
		p.setThetaDegree(thetaDegree);
		p.setPiDegree(piDegree);
		return p;
	}
	
	template<typename T>
	SphericalPoint<T> SphericalPoint<T>::create(const Rectangular3Point<T> &o)
	{
		T x = o.getX();
		T y = o.getY();
		T z = o.getZ();
		
		T r = sqrt(x*x + y*y + z*z);
		if(r == 0)
		{
			//원점인 경우,
			return createDegreeBase(0, 0, 0);
		}
		//c std math func의 반환이니까 rad로 나온다 
		T theta = acos(z/r);
		
		T pi = 0;
		if(x != 0)
		{
			//x가 0이면 y/x를 계산할수 없다. 
			pi = atan(y/x);
		}
		else
		{
			//x가 -이라는것은 pi의 각도는 90이거나 270이라는것이다. 이것은 y의 부호에 따라 결정된다 
			if(y > 0)
			{
				pi = deg2rad(static_cast<T>(90.0f));
			}
			else if(y == 0)
			{
				pi = deg2rad(static_cast<T>(0.0f));
			}
			else
			{
				pi = deg2rad(static_cast<T>(270.0f));
			}
		}
		return createRadianBase(r, theta, pi);
	}

	template<typename T>
	void SphericalPoint<T>::setRadius(T r)
	{
		//radius는 0보다 크거나 같도록 만들기 
		T radius = r > 0 ? r : -r;
		radius_ = radius;
	}
	template<typename T>
	void SphericalPoint<T>::setThetaDegree(T deg)
	{
		//theta의 범위:0~180
		int thetaLoopCnt = deg / 180.0f;
		if(deg < 0)
		{
			//-10 / 180은 0이니까 1을 더해서 넘긴다 
			thetaLoopCnt -= 1;
		}
		thetaDegree_ = deg - thetaLoopCnt * 180.0f;
		MT_ASSERT(thetaDegree_ >= 0 && thetaDegree_ <= 180.0f);
	}
	template<typename T>
	void SphericalPoint<T>::setThetaRadian(T rad)
	{
		setThetaDegree(rad2deg(rad));
	}
	template<typename T>
	void SphericalPoint<T>::setPiDegree(T deg)
	{
		//pi의 범위:0~360 
		int piLoopCnt = deg / 360.0f;
		if(deg < 0)
		{
			piLoopCnt -= 1;
		}
		piDegree_ = deg - piLoopCnt * 360.0f;
		MT_ASSERT(piDegree_ >= 0 && piDegree_ <= 360.0f);
	}
	template<typename T>
	void SphericalPoint<T>::setPiRadian(T rad)
	{
		setPiDegree(rad2deg(rad));
	}
	
	template<typename T>
	T SphericalPoint<T>::getThetaDegree() const
	{
		return thetaDegree_;
	}
	template<typename T>
	T SphericalPoint<T>::getPiDegree() const
	{
		return piDegree_;
	}
	template<typename T>
	T SphericalPoint<T>::getThetaRadian() const
	{
		return degree2radian(thetaDegree_);
	}
	template<typename T>
	T SphericalPoint<T>::getPiRadian() const
	{
		return degree2radian(piDegree_);
	}
	template<typename T>
	T SphericalPoint<T>::getRadius() const
	{
		return radius_;
	}
	
	
	template<typename T>
	Rectangular3Point<T>::Rectangular3Point(T x, T y, T z)
	: x_(x), y_(y), z_(z)
	{
	}
	template<typename T>
	Rectangular3Point<T> Rectangular3Point<T>::create(const SphericalPoint<T> &o)
	{
		//TODO:...
		//구면극좌표를 변환하는 일은 아직 없다. 필요해지면 그떄 구현하자 
		MT_ASSERT(!"TODO: impl");
	}
	
	template<typename T>
	T Rectangular3Point<T>::getX() const
	{
		return x_;
	}
	template<typename T>
	T Rectangular3Point<T>::getY() const
	{
		return y_;
	}
	template<typename T>
	T Rectangular3Point<T>::getZ() const
	{
		return z_;
	}
}
