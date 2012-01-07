/*  Copyright (C) 2011 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#include "yukino_stdafx.h"
#include "sora/clock.h"
#include "sora/gl_helper.h"
#include "sora/texture.h"
#include "sora/filesystem.h"
#include "sora/gl_window.h"
#include "sora/memory_file.h"

#include "sora/immediate_mode_emulator.h"
#include "sora/touch.h"

#include "glassless3d.h"
#include "sora/texture_manager.h"

#include "sora/ui_component.h"
#include "sora/image_label.h"
#include "sora/button.h"

#include "sora/ui_drawer.h"
#include "sora/ui_container.h"
#include "sora/texture_atlas.h"
#include "sora/texture_manager.h"
#include "sora/sprite_sheet_manager.h"

const int win_width = 960;
const int win_height = 640;

void Update(int ms);
void Draw(int ms);
void Init();

void InitWindow(int w, int h);

sora::Texture *tex = NULL;

//ui test
sora::UIContainer ui_container;


int main(int argc, char *argv) {
  //쓰레드 테스트
  //t.timed_join(boost::posix_time::seconds(1));  // 1초 동안 쓰레드 종료 대기
  //for(int i = 0 ; i < 10 ; i++) {
  //  printf("thead 1 %d\n", i);
  //  boost::this_thread::sleep(boost::posix_time::seconds(1)); 
  //}
  //t.join();  
  //return 0;

  // init glfw
  sora::GLWindow win(win_width, win_height, sora::kWinModeWindow, 1);
  win.Init();

  Init();

  sora::TextureManagerThreadRunner texture_thd_runner;
  boost::thread texture_thd(texture_thd_runner);

  // Main loop
  int running = true;
  int prev_runtime = sora::Clock::currtime;
  while( running ) {
    int curr_ms = sora::Clock::Tick();
    int delta_ms = curr_ms - prev_runtime;
    // update timer
    Update(delta_ms);
    Draw(delta_ms);

    // Swap front and back rendering buffers
    glfwSwapBuffers();
    // Check if ESC key was pressed or window was closed
    running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );

    prev_runtime = sora::Clock::currtime;

    //////////
    // engine logic
    sora::TextureManager::GetInstance().ProcessResponse();
  }

  //texture thread stop
  sora::TextureManagerThreadRunner::run_thread = false;
  texture_thd.join();

  // Close window and terminate GLFW
  sora::ImmediateModeEmulator::DestroyInstance();
  
  glfwTerminate();  /////
  // Exit program
  exit( EXIT_SUCCESS );

  return 0;
}

void Update(int ms) {
  //sora::glfwMouseUpdate(win_width, win_height); ???

  yukino::Glassless3d::GetInstance().Update(ms * 0.001f);
}
void Draw(int ms) {
  // OpenGL rendering goes here...
  srglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  yukino::Glassless3d::GetInstance().Draw();
  /*
  //sora::Texture &tex = sora::Texture::Sample();
  srglBindTexture(GL_TEXTURE_2D, tex.handle);

  //set matrix
  srglMatrixMode(SR_PROJECTION);
  srglLoadIdentity();
  srglPerspective(60.0f, 480.0f / 320.0f, 0.1f, 1000.0f);
  srglLookAt(0.0f, 0.0f, 4.0f,    0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f);

  srglMatrixMode(SR_MODELVIEW);
  srglLoadIdentity();
  srglScalef(0.5, 0.5, 0.5);

  ////////////////////////////
  srglBegin(GL_QUADS);
  srglTexCoord2f(0, 0); srglVertex3f(-0.5, -0.5, 0);
  srglTexCoord2f(1, 0); srglVertex3f(0.5, -0.5, 0);
  srglTexCoord2f(1, 1); srglVertex3f(0.5, 0.5, 0);
  srglTexCoord2f(0, 1); srglVertex3f(-0.5, 0.5, 0);
  srglEnd();
  */
  /*
  {
    sora::Texture &color_tex = sora::Texture::White();
    srglBindTexture(GL_TEXTURE_2D, color_tex.handle);
    //2d test
    srglMatrixMode(SR_PROJECTION);
    srglLoadIdentity();
    srglOrtho(-100, 100, -100, 100, -1, 1);

    srglMatrixMode(SR_MODELVIEW);
    srglLoadIdentity();

    srglTranslatef(30, 0, 0);
    srglScalef(0.5, 0.5, 1);
    srglRotatef(15, 0, 0, 1);
    srglBegin(GL_QUADS);
    srglColor3f(1, 0, 0); srglTexCoord2f(0, 0); srglVertex2f(-50, -50);
    srglColor3f(0, 1, 0); srglTexCoord2f(1, 0); srglVertex2f(50, -50);
    srglColor3f(0, 0, 1); srglTexCoord2f(1, 1); srglVertex2f(50, 50);
    srglColor3f(1, 1, 1); srglTexCoord2f(0, 1); srglVertex2f(-50, 50);
    srglEnd();
  }
  */
  //테스트 UI그리기

  sora::UIDrawer drawer;
  drawer.DrawRoot(&ui_container);
  drawer.DrawTouchArea(&ui_container);

  sora::GLHelper::CheckError("DrawEnd");
}

class UIEventTestClass : public sora::Selector {
public:
  virtual void OnButtonPressed(sora::UIComponent *btn) {
    printf("btn pressed\n");
  }
  virtual void OnButtonReleased(sora::UIComponent *btn) {
    printf("btn released\n");
  }
};

void Init() {
  sora::ImmediateModeEmulator::GetInstance().Init();


  srglViewport(0, 0, win_width, win_height);

  //텍스쳐 떄려박는건 좀 무식한거같은데...
  //using std::string;
  //string filename = "\\res\\Scene10.png";
  //string filename = "\\res\\test.png";
  //filename = sora::Filesystem::GetAppPath(filename);
  //tex = new sora::Texture();
  //sora::Texture::LoadFromPNG(filename, tex);

  yukino::Glassless3d::GetInstance().Init();

  //테스트용 UI만들기
  using namespace sora;

  //ui에서 쓰이는 스프라이트 로딩
  MemoryFile sprite_xml_file("ui/ui_sprite-hd.xml");
  sprite_xml_file.Open();
  TextureAtlas tex_atlas = SpriteSheetManager::Read((const char*)sprite_xml_file.start, "/ui/");
  SpriteSheetManager::GetInstance().Save(tex_atlas);
  TextureManager::GetInstance().AsyncLoad(tex_atlas.tex_handle);

  TextureSubImage *next_img = tex_atlas.GetSubImage("BtMainNext@2x");
  TextureSubImage *prev_img = tex_atlas.GetSubImage("BtMainPrev@2x");
  TextureSubImage *reset_img = tex_atlas.GetSubImage("BtMainReset@2x");
  TextureSubImage *menu_img = tex_atlas.GetSubImage("BtMainPage@2x");
  SR_ASSERT(next_img != NULL);
  SR_ASSERT(prev_img != NULL);
  SR_ASSERT(reset_img != NULL);
  SR_ASSERT(menu_img != NULL);

  {
    ImageLabel *label = new ImageLabel(*prev_img);
    label->set_position(vec2(100, 100));
    ui_container.Add(label);
  }
  {
    static UIEventTestClass obj;
    Button *btn = new Button(*next_img);
    btn->set_position(vec2(400, 100));

    UICallbackFunctor pressed_functor(&obj, SR_UI_CALLBACK_SEL(UIEventTestClass::OnButtonPressed));
    btn->set_pressed_functor(pressed_functor);
    UICallbackFunctor released_functor(&obj, SR_UI_CALLBACK_SEL(UIEventTestClass::OnButtonReleased));
    btn->set_released_functor(released_functor);

    btn->set_pressed_img(*prev_img);


    ui_container.Add(btn);
  }
  {
    ImageLabel *label = new ImageLabel(*reset_img);
    label->set_position(vec2(100, 400));
    ui_container.Add(label);
  }
  {
    ImageLabel *label = new ImageLabel(*menu_img);
    label->set_position(vec2(400, 400));
    ui_container.Add(label);
  }
}
