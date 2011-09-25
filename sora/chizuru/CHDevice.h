// Ŭnicode please
#pragma once

#include "sora/SRSingleton.h"

namespace chizuru
{
	class DeviceCore;
	class Device;
	
	///@brief 장치 방향
	typedef enum {
		DeviceOrientationPortrait,
		DeviceOrientationUpsideDownPortrait,
		DeviceOrientationLandscapeLeft,
		DeviceOrientationLandscapeRight
	} DeviceOrientation;
	
	class Device : public sora::Singleton<Device> {
	public:
		Device();
		DeviceOrientation getOrientation() const;
		
		bool isLandscape() const;
		bool isPortrait() const;

		///@brief 매 프레임마다 처음에 호출해서 이벤트를 처리하는 함수 
		void update();
		
		///센서가 뭐 있나 보는거 
		bool isAttitudeAvailable() const;
		bool isGravityAvailable() const;
		bool isAccelerometerAvailable() const ;
	private:
		DeviceCore &core() const;
	};
	
	//device에서 사용하는것에 대한 인터페이스  
	class DeviceCore {
	public:	
		virtual ~DeviceCore() {}
		
		virtual DeviceOrientation getOrientation() const = 0;
		virtual bool isAttitudeAvailable() const = 0;
		virtual bool isGravityAvailable() const = 0;
		virtual bool isAccelerometerAvailable() const = 0;
		
		virtual void update() = 0;
	};
}