// Ŭnicode please
#include "yukino_stdafx.h"

#include "book.h"
#include "camera.h"

#include "renderer.h"

using namespace sora;

namespace yukino {;
Tech48Camera::Tech48Camera()
{
}
Tech48Camera::~Tech48Camera()
{

}
void Tech48Camera::apply(float xRad, float yRad)
{
  //구면위를 따라서 움직이는 느낌의 카메라 도입하기
  float camRadius = Book::GetInstance().getCamRadius();
  float posX = camRadius * sin(xRad) * cos(yRad);
  float posY = camRadius * sin(yRad);
  float posZ = camRadius * cos(xRad) * cos(yRad);

  //원점을 중심으로 오른쪽 방향벡터. 이것을 이용해ㅓ up를 계산한다
  float rightX = cos(xRad);
  float rightY = 0;
  float rightZ = -sin(xRad);

  vec3 posVec(posX, posY, posZ);
  vec3 rightVec(rightX, rightY, rightZ);
  vec3 upVec = sora::Cross(posVec, rightVec);
  Normalized(upVec);

  //@XXX nan?
  /*
  MT_ASSERT(isNaN(posX) == false);
  MT_ASSERT(isNaN(posY) == false);
  MT_ASSERT(isNaN(posZ) == false);
  MT_ASSERT(isNaN(upVec.x) == false);
  MT_ASSERT(isNaN(upVec.y) == false);
  MT_ASSET(isNaN(upVec.z) == false);
  */

  srglLookAt(posX, posY, posZ, 0, 0, 0, upVec.x, upVec.y, upVec.z);
  //printf("%f,%f,%f - %f,%f,%f\n", posX, posY, posZ, upVec.x, upVec.y, upVec.z);
  //mat4 shear;
  //shear.z.x = tan(xTranslateRad);
  //shear.z.y = tan(yTranslateRad);
  //srMultMatrix(shear);

  //카메라를 박스 안쪽으로 밀어넣기...의 수치를 좀 조정할수 있어야될듯?
  srglTranslatef(0, 0, camRadius);

  srglMatrixMode   ( SR_MODELVIEW );  /* Select The Model View Matrix*/
  srglLoadIdentity ( );    /* Reset The Model View Matrix*/
}
void Tech48Camera::setView(float w, float h)
{
#if _WIN_
  //윈도우 에뮬의 경우는 강제로 뒤집고 시작
  float tmp = w;
  w = h;
  h = tmp;
#endif
  glViewport     ( 0, 0, w, h );

  srglMatrixMode   ( SR_PROJECTION );  /* Select The Projection Matrix*/
  srglLoadIdentity ( );                /* Reset The Projection Matrix*/
  if ( h==0 )  /* Calculate The Aspect Ratio Of The Window*/
  {
    srglPerspective ( 80, ( float ) w, 0.1, 100.0 );
  }
  else
  {
    //srPerspective ( 80, ( float ) w / ( float ) h, 0.1, 100.0 );
    srglPerspective ( 60, ( float ) w / ( float ) h, 0.1, 1000 );
  }

  /*아이폰의 경우는 일단 화면을 90도 돌려놓고 진행하자*/
  /*장치의 방향에 맞춰서 회전시키기*/
  /*해도 되지만 현재는 어차피 가로밖에 안쓰니까 이거 하나로 고정하자*/
  /*아래의 코드는 후에 소라코어로 옮기자 */
  /*
  switch(Device::GetInstance().getOrientation()) {
  case DeviceOrientationPortrait:
  break;
  case DeviceOrientationUpsideDownPortrait:
  srRotatef(-180, 0, 0, 1);
  break;
  case DeviceOrientationLandscapeLeft:
  srRotatef(-90, 0, 0, 1);
  break;
  case DeviceOrientationLandscapeRight:
  srRotatef(90, 0, 0, 1);
  break;
  }		
  */
#if _IPHONE_
  //아이폰의 경우는 세로잡기가 opengl의 기본이므로 돌려야된다
  //하지만 윈도우의 경우는 이것없이 해상도조절로 에뮬레이팅 할수있으니까 이건 뺸다
  //srRotatef(90, 0, 0, 1);
#endif
}
}

