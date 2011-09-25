// Ŭnicode please
#pragma once
#if _IPHONE_

#include "chizuru/CHDevice.h"

namespace chizuru
{
	class DeviceCoreIOS : public DeviceCore, public sora::Singleton<DeviceCoreIOS> {
	public:
		DeviceCoreIOS();
		virtual ~DeviceCoreIOS();
		DeviceOrientation getOrientation() const;
		bool isAttitudeAvailable() const;
		bool isGravityAvailable() const;
		bool isAccelerometerAvailable() const;
		
		void update();
		
		void setMotionManager(void *mgr);
	private:
		//for attitude, gyroscope, use core motion
		//http://stackoverflow.com/questions/3245733/apple-gyroscope-sample-code
		//CMMotionManager *motionManager_;
		void *motionManager_;
	};
}
#endif