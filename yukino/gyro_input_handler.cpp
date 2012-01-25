// Ŭnicode please
#include "yukino_stdafx.h"
#include "gyro_input_handler.h"
#include "sora/math_helper.h"
#include "sora/input/gyro.h"
#include "accelerometer_filter.h"
#include "sora/input/accelerometer.h"
#include <memory>

using namespace std;
using namespace sora;

namespace yukino {;

std::auto_ptr<LowpassFilter> gravityLpf;

GyroInputHandler::GyroInputHandler()
: zero_yaw_(0), zero_roll_(0), pan_deg_(0), tilt_deg_(90) {
  static const double kMinCutoffFrequency = 1;
  float maximumValue = 100;
  float value = 80;
  gravityLpf = auto_ptr<LowpassFilter>(new LowpassFilter(maximumValue - value + kMinCutoffFrequency));
}
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
  static double elapsed_time = 1000;
  elapsed_time += 1.0f/30.0f;

  //아이폰4에서는 자이로의 roll을 이용해서 좌우보는것을 구현
  //중력없이 구현하기 
  GyroData attitude = Gyro::GetInstance().GetLast();

  /*
  //필터 거치기
  AccelData tmp_data;
  tmp_data.x = attitude.yaw;
  tmp_data.y = attitude.roll;
  tmp_data.z = attitude.pitch;
  gravityLpf->addAcceleration(tmp_data, elapsed_time);
  attitude.yaw = gravityLpf->x;
  attitude.roll = gravityLpf->y;
  attitude.pitch = gravityLpf->z;
  */

  tilt_deg_ = -RadianToDegree(attitude.roll - zero_roll_);
  float panRadian = CalcIncludeAngleRadian(zero_yaw_, attitude.yaw);
  pan_deg_ = RadianToDegree(panRadian);

  SR_ASSERT(IsNaN(pan_deg_) == false);
  SR_ASSERT(IsNaN(tilt_deg_) == false);
}
void GyroInputHandler::Reset() {
  //자이로센서의 yaw를 저장. 이것을 이용해서 좌우기울이는 효과를 만들어낸다 
  const GyroData &attitude = Gyro::GetInstance().GetLast();
  zero_yaw_ = attitude.yaw;
  zero_roll_ = attitude.roll;
}
}

