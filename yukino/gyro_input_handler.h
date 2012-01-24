#pragma once

#include "input_handler.h"

namespace yukino {;
class GyroInputHandler : public InputHandler {
public:
  GyroInputHandler();
  virtual ~GyroInputHandler();
  virtual float GetPanDegree();
  virtual float GetTiltDegree();
  virtual void UpdateEvent();
  virtual void Reset();
private:
  //기준상태의 중력 
  //for gyro 
  //matsu::vec3 zeroGravity_;
  float zero_yaw_;
  float zero_roll_;

  float pan_deg_;
  float tilt_deg_;
};
}