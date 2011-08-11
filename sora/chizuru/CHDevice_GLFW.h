// Ŭnicode please
#pragma once

#if _WIN_

#include "chizuru/CHDevice.h"

namespace chizuru
{
	///@brief windowns 환경에서 쓰이는것
	class DeviceCoreWin : public DeviceCore {
	public:
		DeviceCoreWin();
		virtual ~DeviceCoreWin();
		
		virtual DeviceOrientation getOrientation() const;
		virtual bool isAttitudeAvailable() const;
		virtual bool isGravityAvailable() const;
		virtual bool isAccelerometerAvailable() const;
		
		virtual void update();
	};
}
#endif