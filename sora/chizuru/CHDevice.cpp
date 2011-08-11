// Ŭnicode please
#include "../SoraStdAfx.h"
#include "chizuru/CHDevice.h"
#include "chizuru/CHDevice_GLFW.h"
#include "chizuru/CHDevice_iOS.h"

namespace chizuru
{
	Device::Device()
	{
	}
	DeviceOrientation Device::getOrientation() const
	{
		return core().getOrientation();
	}
	void Device::update()
	{
		return core().update();
	}
	
	bool Device::isLandscape() const
	{
		return (getOrientation() == DeviceOrientationLandscapeLeft
				|| getOrientation() == DeviceOrientationLandscapeRight);
	}
	bool Device::isPortrait() const
	{
		return !isLandscape();
	}
	
	bool Device::isAttitudeAvailable() const
	{
		return core().isAttitudeAvailable();
	}
	bool Device::isGravityAvailable() const
	{
		return core().isGravityAvailable();
	}
	bool Device::isAccelerometerAvailable() const
	{
		return core().isAccelerometerAvailable();
	}
	
	DeviceCore &Device::core() const
	{
#if _IPHONE_
		return DeviceCoreIOS::getInstance();
#elif _WIN_
		static DeviceCoreWin c;
		return c;
#else
#error "not support"
#endif
	}
}