// Ŭnicode please
#include "yukino_stdafx.h"
#include "accelerometer_input_handler.h"
#include "sora/math_helper.h"
#include "sora/input/accelerometer.h"
#include "sora/vector.h"
#include "sora/coordinate.h"

using namespace std;
using namespace sora;

namespace yukino {;
AccelerometerInputHandler::AccelerometerInputHandler()
  : pan_deg_(0), tilt_deg_(0)
{
  //static const double kUserAccelerationFilterConstant = 0.1;
  static const double kMinCutoffFrequency = 1;
  //static const double kUserAccelerationHpfCutoffFrequency = 1000.0;
  //static const double kUserAccelerationLpfCutoffFrequency = 10.0;

  float maximumValue = 100;
  float value = 80;

  //필터 초기화
  gravityLpf_ = auto_ptr<LowpassFilter>(new LowpassFilter(maximumValue - value + kMinCutoffFrequency));
  //재설정
  //[gravityLpf setCutoffFrequency:(sender.maximumValue - sender.value + kMinCutoffFrequency)];
}
AccelerometerInputHandler::~AccelerometerInputHandler() {

}

float AccelerometerInputHandler::GetPanDegree() {
  SR_ASSERT(IsNaN(pan_deg_) == false);
  return pan_deg_;
}
float AccelerometerInputHandler::GetTiltDegree() {
  SR_ASSERT(IsNaN(tilt_deg_) == false);
  return tilt_deg_;
}
void AccelerometerInputHandler::UpdateEvent() {
  //아이폰3에서는 순수 가속도로 구현
  const AccelData &data = Accelerometer::GetInstance().GetLast();
  vec3 curr(data.x, data.y, data.z);

  //이전 프레임에서의 가속도값과 현재 프레임에서의 가속도값을 차이를 보고,
  //많이 차이나지 않으면 아래의 로직을 적용하지 않는다. 
  //이런거 없이 그냥 적용하면 손떨림떄문에 화면이 덜덜 떨린다....
  //하지만 이렇게 하면 뚝뚝 끊기는 느낌이 너무 강하다. 대안으로 
  //dt이상 변할떄만 움직이는게 아니라 필터를 달아서
  //느리게 변화하는식으로 바꾸기		

  //filter를 통과시켜서 curr를 변화시킨다
  //CMAcceleration accel = {curr.x, curr.y, curr.z};
  gravityLpf_->addAcceleration(data, 0);
  // gravity estimate: [gravityLpf.x, gravityLpf.y, gravityLpf.z]
  curr.x = gravityLpf_->x;
  curr.y = gravityLpf_->y;
  curr.z = gravityLpf_->z;
  if(VectorIsZero(curr, 0.0001)) { 
    curr = vec3(0, 0, -1);
  }
  if(IsNormalNumber(curr.x) == false) {
    curr.x = -1.0f;
  }
  if(IsNormalNumber(curr.y) == false) {
    curr.y = -1.0f;
  }
  if(IsNormalNumber(curr.z) == false) {
    curr.z = -1.0f;
  }
  SR_ASSERT(IsNormalNumber(curr.x));
  SR_ASSERT(IsNormalNumber(curr.y));
  SR_ASSERT(IsNormalNumber(curr.z));

  //zero의 구면극좌표 
  Rectangular3Point<float> zeroRPoint(zero_accel_.x, zero_accel_.y, zero_accel_.z);
  SphericalPoint<float> zeroSPoint = SphericalPoint<float>::Create(zeroRPoint);
  //설마 반지름이 0되는 버그가?
  SR_ASSERT(zeroSPoint.radius() > 0);

  //현재중력의 극좌표 
  Rectangular3Point<float> currRPoint(curr.x, curr.y, curr.z);
  SphericalPoint<float> currSPoint = SphericalPoint<float>::Create(currRPoint);
  //설마 반지름이 0되는 버그가?
  SR_ASSERT(currSPoint.radius() > 0);

  float zeroTheta = zeroSPoint.theta_deg();
  float currTheta = currSPoint.theta_deg();
  tilt_deg_ = (currTheta - zeroTheta);

  float zeroPi = zeroSPoint.pi_deg();
  float currPi = currSPoint.pi_deg();

  pan_deg_ = CalcIncludeAngleDegree(zeroPi, currPi);


  //이전프레임에서의 가속도값을 저장해놓는다. 다음프레임에서 쓰게된다
  //prev = curr;

  SR_ASSERT(IsNaN(pan_deg_) == false);
  SR_ASSERT(IsNaN(tilt_deg_) == false);
}
void AccelerometerInputHandler::Reset() {
  //reset으로 작용하기 위해서 가장 마지막에 추출된 중력값을 임시로 저장한다.
  //이후에는 이 값을 기준으로 화면을 틀어지는것을 구현
  //gyro가 없는 경우...
  const AccelData &data = Accelerometer::GetInstance().GetLast();
  vec3 accel(data.x, data.y, data.z);
  VectorNormalized(accel);
  zero_accel_ = accel;
}

}