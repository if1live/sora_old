// SoraWrapper.h

#pragma once

using namespace System;

namespace sora {;
class Device;
}

namespace SoraWrapper {;
public ref class GLView {;
public:
  GLView();
  ~GLView();
  void SetupGraphics(int w, int h);
  void SetWindowSize(int w, int h);
  void DrawFrame();
  void InitGLEnv();
  void UpdateFrame(float dt);
  void Cleanup();

  void TestDraw(int w, int h);
private:
  sora::Device *device_;
};
}
