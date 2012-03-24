// SoraWrapper.h

#pragma once

using namespace System;

namespace SoraWrapper {
	public ref class GLView {
  public:
    void SetupGraphics(int w, int h);
    void SetWindowSize(int w, int h);
    void DrawFrame();
    void InitGLEnv();
    void UpdateFrame(float dt);
    void Cleanup();
	};
}
