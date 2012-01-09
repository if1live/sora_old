// Ŭnicode please
#include "yukino_stdafx.h"

#include "book.h"
#include "camera.h"

#include "sora/immediate_mode_emulator.h"

#if SR_USE_PCH == 0
#include <cmath>
#endif

using namespace sora;

namespace yukino {;
Tech48Camera::Tech48Camera()
: pos_x_(0), pos_y_(0), pos_z_(0) {
}
Tech48Camera::~Tech48Camera() {
}
void Tech48Camera::Apply(float x_rad, float y_rad) {
  //구면위를 따라서 움직이는 느낌의 카메라 도입하기
  float cam_radius = Book::GetInstance().cam_radius();
  float pos_x = cam_radius * sin(x_rad) * cos(y_rad);
  float pos_y = cam_radius * sin(y_rad);
  float pos_z = cam_radius * cos(x_rad) * cos(y_rad);

  //원점을 중심으로 오른쪽 방향벡터. 이것을 이용해ㅓ up를 계산한다
  float right_x = cos(x_rad);
  float right_y = 0;
  float right_z = -sin(x_rad);

  vec3 pos_vec(pos_z, pos_y, pos_z);
  vec3 right_vec(right_x, right_y, right_z);
  vec3 up_vec = sora::Cross(pos_vec, right_vec);
  Normalized(up_vec);

  //@XXX nan?
  /*
  MT_ASSERT(isNaN(posX) == false);
  MT_ASSERT(isNaN(posY) == false);
  MT_ASSERT(isNaN(posZ) == false);
  MT_ASSERT(isNaN(upVec.x) == false);
  MT_ASSERT(isNaN(upVec.y) == false);
  MT_ASSET(isNaN(upVec.z) == false);
  */

  srglLookAt(pos_x, pos_y, pos_z, 0, 0, 0, up_vec.x, up_vec.y, up_vec.z);
  //printf("%f,%f,%f - %f,%f,%f\n", posX, posY, posZ, upVec.x, upVec.y, upVec.z);
  //mat4 shear;
  //shear.z.x = tan(xTranslateRad);
  //shear.z.y = tan(yTranslateRad);
  //srMultMatrix(shear);

  //카메라를 박스 안쪽으로 밀어넣기...의 수치를 좀 조정할수 있어야될듯?
  //카메라가 이동되어야되는 경우, 게임같은것을 구현하게되면 아마도 쓰겠지?
  srglTranslatef(-pos_x_, -pos_y_, -pos_z_ + cam_radius);

  srglMatrixMode   ( SR_MODELVIEW );  /* Select The Model View Matrix*/
  srglLoadIdentity ( );    /* Reset The Model View Matrix*/
}
void Tech48Camera::SetView(float w, float h) {
  glViewport     ( 0.0f, 0.0f, w, h );

  srglMatrixMode   ( SR_PROJECTION );  /* Select The Projection Matrix*/
  srglLoadIdentity ( );                /* Reset The Projection Matrix*/
  if ( h==0 ) {
    /* Calculate The Aspect Ratio Of The Window*/
    srglPerspective ( 80.0f, ( float ) w, 0.1f, 100.0f );
  } else {
    //srPerspective ( 80, ( float ) w / ( float ) h, 0.1, 100.0 );
    srglPerspective ( 60.0f, ( float ) w / ( float ) h, 0.1f, 1000.0f );
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
void Tech48Camera::SetPosition(float x, float y, float z) {
  pos_x_ = x;
  pos_y_ = y;
  pos_z_ = z;
}
}