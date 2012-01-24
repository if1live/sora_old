#pragma once

namespace yukino {;
class InputHandler {
public:
  InputHandler();
  ~InputHandler();
  virtual float GetPanDegree() = 0;
  virtual float GetTiltDegree() = 0;
  virtual void UpdateEvent() = 0;
  virtual void Reset() = 0;

  static InputHandler *GetLast();
protected:
  static InputHandler *last_handler_;
};
}