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

cvInputStream::cvInputStream(lua_State *L, String instance) {
  int stack_count = lua_gettop(L);
  printf("Current stack top: %d\n", stack_count);
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
  debug = lua_toboolean(L,-1);
  lua_pop(L,1);

  lua_pushstring(L, "camid");
  lua_gettable(L, -2);
  assert(lua_isnumber(L,-1));
  int camid = lua_tonumber(L,-1);
  lua_pop(L,1);
  
  lua_pushstring(L, "cascadeprofiling");
  lua_gettable(L, -2);
  assert(lua_isboolean(L,-1));
  int cprofile = lua_toboolean(L,-1);
  lua_pop(L,1);

  printf("cvInputStream: %s (%d) %dx%d\n", instance.data(), camid, w, h);
  printf("cvInputStream: Adjusting delays to match %d fps\n", fps);
  framedelay = 1000 / fps;
  capture = cvCaptureFromCAM(camid);
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, w); 
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, h); 
  if (debug) {
    wname = String("cvInputStream: ") + instance;
    wpname = String("cvInputStream: ") + instance + String(" [processed] ");
    printf("cvInputStream: Debug enabled to window '%s'\n", wname.data());
    cvNamedWindow( wname.data(), 1 );
    cvNamedWindow( wpname.data(), 1 );
    }

  /* Now we have to load cascades from config */ 
  lua_pushstring(L, "detect");
  lua_gettable(L, -2);
  assert(lua_istable(L,-1));
  /* Iterate over table members */
  int i=1;
  while(1)
    {
      lua_pushnumber(L, i++);
      lua_gettable(L, -2);
      if (!lua_isstring(L,-1)) {
	lua_pop(L,1);
	break;
	}
      const char* s = lua_tostring(L,-1);
      detectable *dtct = new detectable(L,String(s));
      detectables.push_back(dtct);
      lua_pop(L,1);
    }  
  /* pop remaining 2 elements: detect, and instance */
  lua_pop(L,2);
  stack_count = lua_gettop(L);
  printf("Current stack top: %d\n", stack_count);
}

/* process next frame actually */ 
void cvInputStream::processNextFrame(lua_State* L){
  IplImage* iplImg = cvQueryFrame( capture );
  int i;
  Mat img = iplImg;
  Mat gray(img.rows, img.cols, CV_8UC1);
  
  cvtColor( img, gray, CV_BGR2GRAY );
  equalizeHist( gray, gray );
 if (debug)
    {
    cv::imshow(wname, img);
    cv::imshow(wpname, gray);
    }
  /* Now send it to detectables */
  for (i=0; i<detectables.size(); i++) {
    detectables.at(i)->detect(gray,L);
  }
  waitKey(framedelay);
}
