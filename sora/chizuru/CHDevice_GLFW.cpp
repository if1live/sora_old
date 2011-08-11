// Ŭnicode please
#include "../SoraStdAfx.h"
#include "chizuru/CHDevice_GLFW.h"

#if _WIN_
namespace chizuru
{
	
	DeviceCoreWin::DeviceCoreWin()
	{
	}
	DeviceCoreWin::~DeviceCoreWin()
	{
	}
		
	DeviceOrientation DeviceCoreWin::getOrientation() const
	{
		return DeviceOrientationPortrait;
	}
	bool DeviceCoreWin::isAttitudeAvailable() const
	{
		return false;
	}
	bool DeviceCoreWin::isGravityAvailable() const
	{
		return false;
	}
	bool DeviceCoreWin::isAccelerometerAvailable() const
	{
		return false;
	}
	
	void DeviceCoreWin::update()
	{
		//glfw를 기반으로 키보드/마우스 이벤트 적절히 잡아서
		//이벤트 큐에 집어넣기
	}
}
#endif
