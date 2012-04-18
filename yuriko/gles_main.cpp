#if 0
#include "sora/renderer/gl_inc.h"
#include <stdlib.h>
#include "esUtil.h"
#include "esUtil_win.h"

#include "sora/sora_main.h"
#include "sora/sys/device.h"
#include "sora/event/keyboard_event.h"
#include "sora/core/timer.h"
#include "sora/shadow_map_main.h"
#include "sora/test_function.h"
#include "sora/selection_main.h"
#include "sora/celshading_main.h"
#include "sora/depth_map_main.h"

#include "sora/event/touch_device.h"

#include <queue>

using namespace sora;
using namespace std;

sora::Device dev;
int elapsed_tick_count = 0;
float elapsed_time = 0;

void mouseFunc(int state, int x, int y) {
  TouchDevice &touch_device = TouchDevice::GetInstance();
  int left_state = 0;
  if(state == ES_MOUSE_DOWN || state == ES_MOUSE_MOVE) {
    left_state = kInputPress;
  } else {
    left_state = kInputRelease;
  }
  touch_device.UpdateState(left_state, x, y, elapsed_tick_count, elapsed_time);
  const vector<TouchEvent> &touch_evt_list = touch_device.GetCreatedEvent();
  auto touch_evt_it = touch_evt_list.begin();
  auto touch_evt_endit = touch_evt_list.end();
  TouchEventQueue &touch_evt_queue = dev.touch_evt_queue();
  for( ; touch_evt_it != touch_evt_endit ; ++touch_evt_it) {
    touch_evt_queue.Push(*touch_evt_it);
  }
}

void keyFunc(ESContext *esContext, int state, int virtual_key) {
  KeyboardEvent evt;
  if(state == ES_KEY_DOWN) {
    evt.state = kKeyboardPress;
  } else {
    evt.state = kKeyboardRelease;
  }

  int keycode = virtual_key;
  //virtual key를 적절히 쓸수잇는 코드로 변환
  //http://winapi.co.kr/reference/Message/VirtualKey.htm
  bool is_special = false;
  int arrow_table[][2] = {
    { VK_UP, KeyboardEvent::kUp },
    { VK_DOWN, KeyboardEvent::kDown },
    { VK_LEFT, KeyboardEvent::kLeft },
    { VK_RIGHT, KeyboardEvent::kRight },
  };
  for(size_t i = 0 ; i < sizeof(arrow_table) / sizeof(arrow_table[0]) ; i++) {
    if(virtual_key == arrow_table[i][0]) {
      is_special = true;
      keycode = arrow_table[i][1];
      break;
    }
  }
  
  evt.is_special_key = is_special;
  evt.ch = keycode;
  KeyboardEventQueue &evt_queue = dev.keyboard_evt_queue();
  evt_queue.Push(evt);
}


void celshading_draw(ESContext *esContext) {
  static float prev_time = Timer_GetSecond();
  sora::celshading::draw_frame(&dev);

  float curr_time = Timer_GetSecond();
  float dt = curr_time - prev_time;
  sora::celshading::update_frame(&dev, dt);

  eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
  Timer_Tick();
  prev_time = curr_time;

  elapsed_tick_count++;
  elapsed_time += dt;
  dev.EndTick();
}

void depthmap_draw(ESContext *esContext) {
  static float prev_time = Timer_GetSecond();
  sora::depthmap::draw_frame(&dev);

  float curr_time = Timer_GetSecond();
  float dt = curr_time - prev_time;
  sora::depthmap::update_frame(&dev, dt);

  eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
  Timer_Tick();
  prev_time = curr_time;

  elapsed_tick_count++;
  elapsed_time += dt;
  dev.EndTick();
}

void selection_draw(ESContext *esContext) {
  static float prev_time = Timer_GetSecond();
  sora::selection::draw_frame(&dev);

  float curr_time = Timer_GetSecond();
  float dt = curr_time - prev_time;
  sora::selection::update_frame(&dev, dt);

  eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
  Timer_Tick();
  prev_time = curr_time;

  elapsed_tick_count++;
  elapsed_time += dt;
  dev.EndTick();
}

void main_draw(ESContext *esContext) {
  static float prev_time = Timer_GetSecond();
  SORA_draw_frame(&dev);

  float curr_time = Timer_GetSecond();
  float dt = curr_time - prev_time;
  SORA_update_frame(&dev, dt);

  eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
  Timer_Tick();
  prev_time = curr_time;

  elapsed_tick_count++;
  elapsed_time += dt;
  dev.EndTick();
}


int main ( int argc, char *argv[] ) {
  ESContext esContext;
  esInitContext ( &esContext );

  int kWinWidth = 480;
  int kWinHeight = 800;

  esCreateWindow ( &esContext, "Hello Triangle", kWinWidth, kWinHeight, ES_WINDOW_RGB );

  //connect input func
  esRegisterMouseFunc(&esContext, mouseFunc);
  esRegisterKeyFunc(&esContext, keyFunc);

  //init env
  Timer_Init();

  SORA_setup_graphics(&dev, kWinWidth, kWinHeight);
  esRegisterDrawFunc ( &esContext, main_draw );
  //sora::celshading::setup_graphics(&dev, kWinWidth, kWinHeight);
  //esRegisterDrawFunc ( &esContext, celshading_draw );
  //sora::depthmap::setup_graphics(&dev, kWinWidth, kWinHeight);
  //esRegisterDrawFunc ( &esContext, depthmap_draw);
  //sora::selection::setup_graphics(&dev, kWinWidth, kWinHeight);
  //esRegisterDrawFunc ( &esContext, selection_draw);
  
  esMainLoop ( &esContext );
}
#endif