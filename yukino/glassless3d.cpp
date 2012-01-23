/* Ŭnicode please */
#include "yukino_stdafx.h"

#include "glassless3d.h"
#include "sora/render/immediate_mode_emulator.h"
#include "sora/render/matrix_stack.h"

#include "camera.h"
#include "book_scene.h"
#include "book.h"

#include "sora/filesystem.h"
#include "sora/math_helper.h"
#include "sora/sys_locale.h"
#include "sora/render/gl_window.h"

#if SR_USE_PCH == 0
#include "sora/render/gl_inc.h"
#endif

//input
//#include "ASInputHandler.h"
//#include "ASKeyboardInputHandler.h"
//#include "ASGyroInputHandler.h"
//#include "ASAccelerometerInputHandler.h"

namespace yukino {;

void Glassless3d::Init() {
  using namespace sora;
  using namespace std;

  /*여기에서 사용할 모델을 미리 만들어놓자 */
  InitGrid();
  //initBookScene();	//book scene로딩은 밖에서 결정하도록 고치자
  /*initTargetModel();*/

  //event handler 생성 
#if SR_IOS
  if(Device::GetInstance().isAttitudeAvailable()) {
    handler_ = auto_ptr<InputHandler>(new GyroInputHandler());
    //handler_ = auto_ptr<InputHandler>(new AccelerometerInputHandler());
  } else {		
    handler_ = auto_ptr<InputHandler>(new AccelerometerInputHandler());
  }
#elif SR_WIN
  //handler_ = auto_ptr<InputHandler>(new KeyboardInputHandler()); 
#else
//#error "not support this platform"
#endif

#if 0
  //테스트용 큐브 생성
  CubeBuilder builder;
  builder.setSize(10).setRandomColor();
  SoraModel c = builder.buildSolid(SORA_USE_COLOR);
  cube_ = auto_ptr<SoraModel>(new SoraModel(c));
#endif

}

void Glassless3d::Draw() {
  if(visible_ == false) {
    return;
  }

  using namespace sora;

  BookScene *scene = Book::GetInstance().GetCurrScene();

  /*기본 색 정보 날리기*/
  srglClearColor (0, 0, 0, 1);
  /* Clear the color buffer*/
  srglClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //viewing 최초 설정
  ///@TODO retina나 기타 다른 기계때문에 해상도는 잘 처리해야한다
  GLWindow &win = GLWindow::GetInstance();
  int w = win.width();
  int h = win.height();
  //const Viewport &vp = Window::GetInstance().getViewport(1);	//사용하고자하는 glview의 크기

  //카메라 적용
  {
    //카메라 설정..
    Tech48Camera cam;
    cam.SetView((float)w, (float)h);

    ///vertex없이 z값에 따라서 x,y를 움직이게 하는 행렬을 쓰면 적절히 그릴수있다
    //일정각도 이상으로는 안넘어가도록 하자
    //float panDegree = handler_->getPanDegree();
    //float tiltDegree = handler_->getTiltDegree();
    //float pan_deg = 15;
    float pan_deg = 0;
    float tilt_deg = 15;
    static float dynamic_pan_dt = 0;
    dynamic_pan_dt += 0.1f;
    pan_deg = 15 * sin(DegreeToRadian(dynamic_pan_dt));


    pan_deg = Book::GetInstance().CalcPanDeg(pan_deg);
    tilt_deg = Book::GetInstance().CalcTiltDeg(tilt_deg);

    float pan_rad = DegreeToRadian(pan_deg);
    float tilt_rad = DegreeToRadian(tilt_deg);

    //구면위를 따라서 움직이는 느낌의 카메라 도입하기
    //화면을 landscape로 ui쪽에서 설정하면서 방향이 뒤틀렸다. 가장 간단한 편법으로 x,y를 뒤바꿨다
    cam.Apply(pan_rad, tilt_rad);
  }

  srglMatrixMode(SR_MODELVIEW);
  srglLoadIdentity();					/* Reset The Current Modelview Matrix*/

  srglPushMatrix();
  {
    //약간 작게 만들어놓가 사용하자
    //이렇게 화면의 의도적으로 작게 그리게 하고
    //큐브내부에서 뭘 그리거나할때는 일반적으로 사용하는 픽셀같은 느낌의
    //크기로 만들자
    //srScalef(0.1, 0.1, 0.1);

    srglBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //배경과 관련된것 그리기 
    srglDisable(GL_BLEND);

    srglPushMatrix();
    //grid정의 있으면 그리기
    if(scene != NULL && scene->isUseGrid() == true) {
      //grid를 약간 작게 그려야 z-fighting문제가 없다
      srglScalef(0.99f, 0.99f, 0.99f);
      //grid 그리기..어차피 디버깅모드에서만 쓰는거니까 그리는 순서는 신경쓰지 말자 
      //sora::model::ModelUtil::util().draw(gridModel_);
    }
    srglPopMatrix();

    /*glEnable(GL_DEPTH_TEST);*/
    /*모델 그리기 */
    /*텍스쳐 사용 선언*/
    glEnable(GL_BLEND);
    //book scene 적절히 그리기 
    //glDisable(GL_DEPTH_TEST); 
    if(scene != NULL) {
      scene->draw();
    }

    //테스트용 큐브 그리기 
    //colorProg->use();
    //srPushMatrix();
    //srTranslatef(-40, -40, -40);
    //sora::model::ModelUtil::util().draw(*cube_);
    //srPopMatrix();
  }
  srglPopMatrix();
}

Glassless3d::Glassless3d()
  : visible_(true) {
}
Glassless3d::~Glassless3d() {
}

void Glassless3d::Update(float dt) {
  if(visible_ == false) {
    return;
  }
}

void Glassless3d::InitGrid() {
#if 0
  const float cube_width = Book::GetInstance().getWidth();
  const float cube_height = Book::GetInstance().getHeight();
  const float cube_depth = Book::GetInstance().getDepth();
  const float maxDepth = -cube_depth;

  sora::model::SimpleModelBuilder builder(SORA_LINES, SORA_USE_COLOR);

  builder.color4(1.0f, 1.0f, 1.0f, 1.0f);

  //10픽셀 기준으로 선을 긋자
  const float gridSize = 20;
  for(float z = 0 ; z >= -cube_depth ; z-=gridSize)
  {
    builder.vertex3( cube_width/2.0f, cube_height/2.0f, z);		
    builder.vertex3(-cube_width/2.0f, cube_height/2.0f, z);

    builder.vertex3(-cube_width/2.0f, cube_height/2.0f, z);
    builder.vertex3(-cube_width/2.0f,-cube_height/2.0f, z);

    builder.vertex3(-cube_width/2.0f,-cube_height/2.0f, z);
    builder.vertex3(cube_width/2.0f,-cube_height/2.0f, z);

    builder.vertex3(cube_width/2.0f,-cube_height/2.0f, z);
    builder.vertex3( cube_width/2.0f, cube_height/2.0f, z);
  }

  //현재지점에서 안으로 들어가는 선을 그려서 깊이를 더 자세하게 표현
  const int gridSplit = 10;	
  for(float x = -cube_width/2 ; x <= cube_width/2 ; x += gridSize)
  {
    /*top*/
    builder.vertex3(x, cube_height/2.0f, 0);
    builder.vertex3(x, cube_height/2.0f, maxDepth);
    /*bottom*/
    builder.vertex3(x, -cube_height/2.0f, 0);
    builder.vertex3(x, -cube_height/2.0f, maxDepth);
  }
  for(float y = -cube_height/2 ; y <= cube_height/2 ; y += gridSize)
  {
    /*left*/
    builder.vertex3(-cube_width/2.0f, y, 0);
    builder.vertex3(-cube_width/2.0f, y, maxDepth);
    /*right*/
    builder.vertex3(cube_width/2.0f, y, 0);
    builder.vertex3(cube_width/2.0f, y, maxDepth);
  }

  /*맨 뒤에 존재하는 격자 */
  for(float x = -cube_width/2 ; x <= cube_width/2 ; x += gridSize)
  {
    /*세로 */
    builder.vertex3(x, +cube_height/2.0f, maxDepth);
    builder.vertex3(x, -cube_height/2.0f, maxDepth);	
  }
  for(float y = -cube_height/2 ; y <= cube_height/2 ; y += gridSize)
  {
    /*가로선 */
    builder.vertex3(-cube_width/2.0f, y, maxDepth);
    builder.vertex3(+cube_width/2.0f, y, maxDepth);
  }	
  gridModel_ = builder.build();
#endif
}
}