// Ŭnicode please
#pragma once

namespace yukino {;
///@brief tech48과 같이 창문을 통해서 들여다보는 느낌의 카메라이다
///각도 2개를 입력받아서 적절히 계산해서 gluLookAt를 
///호출한다
class Tech48Camera {
public:
  Tech48Camera();
  ~Tech48Camera();

  ///@brief 최초에 화면을 그리지 전에 어떻게 그릴지에 대한 기본 명세
  ///예를 들면, viewport, projection을 설정하는 함수 
  void setView(float w, float h);
  ///@brief gluLookAt를 호출하는 메소드 
  void apply(float xRad, float yRad);
private:
};
}