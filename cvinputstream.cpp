#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}

using namespace std;
using namespace cv;

#include "inputstream.hpp"
#include "cvinputstream.hpp"

/* 
   cvInputStream::cvInputStream(){
   printf("Creating cvInputStream Instance...\n");
   }
*/


int cvInputStream::loadConfigFromLua(lua_State *L, String instance) {
  lua_getglobal(L,instance.data());
  assert(lua_istable(L, -1));

  lua_pushstring(L, "w");
  lua_gettable(L, -2);
  assert(lua_isnumber(L,-1));
  int w = lua_tonumber(L,-1);
  lua_pop(L,1);

  lua_pushstring(L, "h");
  lua_gettable(L, -2);
  assert(lua_isnumber(L,-1));
  int h = lua_tonumber(L,-1);
  lua_pop(L,1);

  lua_pushstring(L, "maxfps");
  lua_gettable(L, -2);
  assert(lua_isnumber(L,-1));
  int fps = lua_tonumber(L,-1);
  lua_pop(L,1);

  lua_pushstring(L, "debug");
  lua_gettable(L, -2);
  assert(lua_isboolean(L,-1));
  int debug = lua_toboolean(L,-1);
  lua_pop(L,1);

  lua_pushstring(L, "camid");
  lua_gettable(L, -2);
  assert(lua_isnumber(L,-1));
  int camid = lua_tonumber(L,-1);
  lua_pop(L,1);
  
  lua_pushstring(L, "cascadeprofiling");
  lua_gettable(L, -2);
  assert(lua_isbolean(L,-1));
  int cprofile = lua_tobolean(L,-1);
  lua_pop(L,1);

  printf("cvInputStream: %s (%d) %dx%d\n", instance.data(), camid, w, h);
  printf("cvInputStream: Adjusting delays to match %d fps\n", fps);
  capture = cvCaptureFromCAM(camid);
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, w); 
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, h); 
  if (debug) {
    String wname = String("cvInputStream: ") + instance;
    printf("cvInputStream: Debug enabled to window '%s'\n", wname.data());
    cvNamedWindow( wname.data(), 1 );
    }
  sleep(100);
  /* Now we have to load cascades from config */ 
  
}

/* process next frame actually */ 
Mat cvInputStream::getNextFrame(){
  
  return Mat();
}
