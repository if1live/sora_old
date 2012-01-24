// Ŭnicode please
#include "yukino_stdafx.h"
#include "gyro_input_handler.h"
#include "sora/math_helper.h"
#include "sora/input/gyro.h"

using namespace std;
using namespace sora;

namespace yukino {;
GyroInputHandler::GyroInputHandler()
  : zero_yaw_(0), zero_roll_(0), pan_deg_(0), tilt_deg_(90) { }
GyroInputHandler::~GyroInputHandler() { }

float GyroInputHandler::GetPanDegree() {
  SR_ASSERT(IsNaN(pan_deg_) == false);
  return pan_deg_;
}
float GyroInputHandler::GetTiltDegree() {
  SR_ASSERT(IsNaN(tilt_deg_) == false);
  return tilt_deg_;
}
void GyroInputHandler::UpdateEvent() {
  /*
  while(InputEventQueue::sharedQueue().isEmpty() == false)
  {
    InputEventPtr evt = InputEventQueue::sharedQueue().pop();
    if(evt->getType() == EventTypeAttitude)
    {
      //아이폰4에서는 자이로의 roll을 이용해서 좌우보는것을 구현
      //중력없이 구현하기 
      AttitudeData *data = evt->getEventData<AttitudeData>();
      tiltDegree_ = -rad2deg(data->roll - zeroRoll_);
      float panRadian = calcIncludeAngleRadian(zeroYaw_, data->yaw);
      panDegree_ = rad2deg(panRadian);

      MT_ASSERT(isNaN(panDegree_) == false);
      MT_ASSERT(isNaN(tiltDegree_) == false);
    }
  }
  */
}
void GyroInputHandler::Reset() {
  //자이로센서의 yaw를 저장. 이것을 이용해서 좌우기울이는 효과를 만들어낸다 
  const GyroData &attitude = Gyro::GetInstance().GetLast();
  zero_yaw_ = attitude.yaw;
  zero_roll_ = attitude.roll;
}
}

