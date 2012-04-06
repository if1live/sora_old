#include "sora/renderer/gl_inc.h"
#include <stdlib.h>
#include "esUtil.h"
#include "esUtil_win.h"

#include "sora/sora_main.h"
#include "sora/sys/device.h"
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
}


int main ( int argc, char *argv[] ) {
  ESContext esContext;
  esInitContext ( &esContext );

  int kWinWidth = 480;
  int kWinHeight = 800;

  esCreateWindow ( &esContext, "Hello Triangle", kWinWidth, kWinHeight, ES_WINDOW_RGB );

  //connect input func
  esRegisterMouseFunc(&esContext, mouseFunc);

  //init env
  Timer_Init();

  //sora::celshading::setup_graphics(&dev, kWinWidth, kWinHeight);
  //esRegisterDrawFunc ( &esContext, celshading_draw );
  sora::depthmap::setup_graphics(&dev, kWinWidth, kWinHeight);
  esRegisterDrawFunc ( &esContext, depthmap_draw);
  //sora::selection::setup_graphics(&dev, kWinWidth, kWinHeight);
  //esRegisterDrawFunc ( &esContext, selection_draw);
  
  esMainLoop ( &esContext );
}
