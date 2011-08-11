#include "chizuru/CHDevice.h"

#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>
#include "mio/MioMacro.h"
#include "chizuru/CHInputEvent.h"
#include "chizuru/CHInputEventQueue.h"

namespace chizuru
{	
	////////////////
	DeviceCoreIOS::DeviceCoreIOS()
	: motionManager_(NULL)
	{
		//motion manager를 여기에서 생성하면 죽는다. -_-
		//그래서 core motion manager은 view controller에서 하고
		//생성된것을 이 클래스로 넘기면 그떄부터 이것이 처리한다
	}
	DeviceCoreIOS::~DeviceCoreIOS()
	{
		//motion disable
		[(CMMotionManager*)motionManager_ release];
		motionManager_ = NULL;
	}
	void DeviceCoreIOS::setMotionManager(void *mgr)
	{
		CMMotionManager* motionManager = (CMMotionManager*)mgr;
		[motionManager retain];
		motionManager_ = motionManager;
		
		//attitude 초기화...gyroscope사용하기
		//motion manager은 시스템상에 1개만 존재해야한다. 적절히 만들것 	
		//CMMotionManager* motionManager = [[CMMotionManager alloc] init];
		//motionManager_ = motionManager;
		//CMMotionManager *motionManager = motionMgr;
		//CMMotionManager* motionManager = (CMMotionManager*)motionManager_;
		
		///@TODO engine이 motion을 필요로하는것을 확인하고 활성화
		if([motionManager isDeviceMotionAvailable]) {
			[motionManager startDeviceMotionUpdates];
			MIO_LOG("use device motion");
		} else {
			MIO_LOG("Not support device motion");
		}
		
		if([motionManager isAccelerometerAvailable]) {
			[motionManager startAccelerometerUpdates];
			MIO_LOG("use accelerometer");
		} else {
			MIO_LOG("Not support accelerometer");
		}
	}
	void DeviceCoreIOS::update()
	{
		if(motionManager_ == NULL)
		{
			return;
		}
		CMMotionManager* motionManager = (CMMotionManager*)motionManager_;
		
		if([motionManager isDeviceMotionActive]) {
			float x = motionManager.deviceMotion.gravity.x;
			float y = motionManager.deviceMotion.gravity.y;
			float z = motionManager.deviceMotion.gravity.z;
			if(!(x == 0 && y == 0 && z == 0)) {
				//무중력인 경우는 이벤트를 생성하지 말자.
				InputEventPtr evt = createGravityEvent(x, y, z);
				InputEventQueue::sharedQueue().push(evt);
			}
			
			//자세 정보를 사용
			float roll = motionManager.deviceMotion.attitude.roll;
			float pitch = motionManager.deviceMotion.attitude.pitch;
			float yaw = motionManager.deviceMotion.attitude.yaw;
			InputEventPtr evt = createAttitudeEvent(roll, pitch, yaw);
			InputEventQueue::sharedQueue().push(evt);
			
			//가속도 센서 살아있으면 그거 쓰기
			float accelx = motionManager.accelerometerData.acceleration.x;
			float accely = motionManager.accelerometerData.acceleration.y;
			float accelz = motionManager.accelerometerData.acceleration.z;
			double timestamp = motionManager.deviceMotion.timestamp;
			InputEventPtr accelevt = createAccelerometerEvent(accelx, accely, accelz, timestamp);
			InputEventQueue::sharedQueue().push(accelevt);
		}
		else
		{
			//core motion의 device motion을 쓰지 못하는경우 낱개로 처리
			//가속도 센서 살아있으면 그거 쓰기
			if([motionManager isAccelerometerActive]) {
				float x = motionManager.accelerometerData.acceleration.x;
				float y = motionManager.accelerometerData.acceleration.y;
				float z = motionManager.accelerometerData.acceleration.z;
				
				//timestamp는 현재시간으로 쓰자
				double timestamp = [[NSDate date] timeIntervalSince1970];
				InputEventPtr evt = createAccelerometerEvent(x, y, z, timestamp);
				InputEventQueue::sharedQueue().push(evt);
			}
		}
	}
	DeviceOrientation DeviceCoreIOS::getOrientation() const
	{
		MT_ASSERT(motionManager_ != NULL);
		//매 프레임을 그리기 전에 아이폰 한정 update코드 실행
		UIDeviceOrientation deviceOrientation = [UIDevice currentDevice].orientation;
		switch(deviceOrientation) {
			case UIDeviceOrientationPortrait:
				return DeviceOrientationPortrait;
			case UIDeviceOrientationPortraitUpsideDown:
				return DeviceOrientationUpsideDownPortrait;
			case UIDeviceOrientationLandscapeLeft:
				return DeviceOrientationLandscapeLeft;
			case UIDeviceOrientationLandscapeRight:
				return DeviceOrientationLandscapeRight;
		}
		//do not reach
		MT_ASSERT(!"do not reach");
		return DeviceOrientationPortrait;
	}
	bool DeviceCoreIOS::isAttitudeAvailable() const
	{
		MT_ASSERT(motionManager_ != NULL);
		CMMotionManager* motionManager = (CMMotionManager*)motionManager_;
		return [motionManager isDeviceMotionAvailable];
	}
	bool DeviceCoreIOS::isGravityAvailable() const
	{
		MT_ASSERT(motionManager_ != NULL);
		CMMotionManager* motionManager = (CMMotionManager*)motionManager_;
		return [motionManager isDeviceMotionAvailable];
	}
	bool DeviceCoreIOS::isAccelerometerAvailable() const
	{
		MT_ASSERT(motionManager_ != NULL);
		CMMotionManager* motionManager = (CMMotionManager*)motionManager_;
		return [motionManager isAccelerometerAvailable];
	}
}