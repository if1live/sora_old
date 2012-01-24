#pragma once

#include "input_handler.h"
#include "accelerometer_filter.h"
#include "sora/vector.h"
#include <memory>

namespace yukino {;
class AccelerometerInputHandler : public InputHandler {
public:
  AccelerometerInputHandler();
  virtual ~AccelerometerInputHandler();
  virtual float GetPanDegree();
  virtual float GetTiltDegree();
  virtual void UpdateEvent();
  virtual void Reset();

private:	
  //for old device 
  sora::vec3 zero_accel_;

  float pan_deg_;
  float tilt_deg_;

  // Low-pass filter for raw accelerometer data
  // Only used the accelMode==YES	 
  std::auto_ptr<LowpassFilter> gravityLpf_;
};
}