#include "sora/renderer/gl_inc.h"
#include <stdlib.h>
#include "esUtil.h"

#include "sora/sora_main.h"
#include "sora/sys/device.h"
#include "sora/core/timer.h"
#include "sora/shadow_map_main.h"
#include "sora/test_function.h"
#include "sora/selection_main.h"
#include "sora/celshading_main.h"
#include "sora/depth_map_main.h"

sora::Device dev;

void celshading_draw(ESContext *esContext) {
  static float prev_time = Timer_GetSecond();
  sora::celshading::draw_frame(&dev);

  float curr_time = Timer_GetSecond();
  float dt = curr_time - prev_time;
  sora::celshading::update_frame(&dev, dt);

  eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
  Timer_Tick();
  prev_time = curr_time;
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
}


int main ( int argc, char *argv[] ) {
  ESContext esContext;
  esInitContext ( &esContext );

  int kWinWidth = 480;
  int kWinHeight = 800;

  esCreateWindow ( &esContext, "Hello Triangle", kWinWidth, kWinHeight, ES_WINDOW_RGB );

  //init env
  Timer_Init();

  //sora::celshading::setup_graphics(&dev, kWinWidth, kWinHeight);
  //esRegisterDrawFunc ( &esContext, celshading_draw );
  sora::depthmap::setup_graphics(&dev, kWinWidth, kWinHeight);
  esRegisterDrawFunc ( &esContext, depthmap_draw);
  esMainLoop ( &esContext );
}
