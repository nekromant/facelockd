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
#include "detectable.hpp"
#include "lua_helpers.h"

void detectable::parse_flag(const char* flag){
  if (0==strcmp(flag, "scale")) {
    flags |= CV_HAAR_SCALE_IMAGE;
  } else if (0==strcmp(flag, "rough")) {
    flags |= CV_HAAR_DO_ROUGH_SEARCH;
  } else if (0==strcmp(flag, "canny")) {
    flags |= CV_HAAR_DO_CANNY_PRUNING;
  } else if (0==strcmp(flag, "biggest")) {
    flags |= CV_HAAR_FIND_BIGGEST_OBJECT;
  }else {
    printf("Fatal: don't know flag '%s'\n", flag);
  }
}

detectable::detectable(lua_State* L, String instance){
  printf("Loading a detectable object: %s\n", instance.data());
  iname = instance;
  lua_getglobal(L,instance.data());
  assert(lua_istable(L, -1));
  
  get_boolean_field(enabled);
  get_boolean_field(debug);
  get_int_field(nearobjs);

  /* Now read out the cascades */
  lua_pushstring(L, "cascades");
  lua_gettable(L, -2);
  int i = 1;
  const char *s;  
  while (1)
    {
      get_str_member(s,i++, lua_pop(L, 1); break; );
      printf("Loading cascade: %s\n", s);
      CascadeClassifier *cascade = new CascadeClassifier;
      assert(cascade->load(s));
      cascades.push_back(cascade);
    }
  lua_pop(L,1);
  int w,h;
  lua_pushstring(L, "minsize");
  lua_gettable(L, -2);
  assert(lua_istable(L, -1));
  get_int_member(w,1, assert(0););
  get_int_member(h,2, assert(0););
  lua_pop(L,1);
  minsz = new Size(w,h);
  /* Now let's load the handlers */

  lua_pushstring(L, "flags");
  lua_gettable(L, -2);
  while (1)
    {
      get_str_member(s,i++, lua_pop(L, 1); break; );
      printf("Adding flag: %s\n", s);
      parse_flag(s);     
    } 
  lua_pop(L,1);
  lua_pushstring(L, "simple_handlers");
  lua_gettable(L, -2);
  assert(lua_istable(L, -1));
  i=1;
  while (1)
    {
      get_str_member(s,i++, lua_pop(L, 1); break; );
      printf("Attaching simple handler: %s\n", s);
      simple_handlers.push_back(String(s));     
    } 
  lua_pop(L,1);
  lua_pushstring(L, "shape_handlers");
  lua_gettable(L, -2);
  assert(lua_istable(L, -1));
  i=1;
  while (1)
    {
      get_str_member(s,i++, lua_pop(L, 1); break; );
      printf("Attaching shape handler: %s\n", s);
      shape_handlers.push_back(String(s));     
    } 
  lua_pop(L,1);  
  printf("Detectable: %s (%dx%d); nearobjs %d; debug: %d; enabled: %d\n", 
	 instance.data(), w, h, nearobjs, debug, enabled);
  lua_pop(L,1); 
  if (debug)
    {
      wname = String("detectable: ") + instance;
      cvNamedWindow(wname.data(), 1);
    }
    
  flags=0; /*TODO: flags parsing */

}

vector<Rect> detectable::detect(Mat& frame, lua_State *L) {
  /* Since we're not going to remove objects in runtime, this is ok */
  vector<Rect> rfaces;  
  int i, j;
  //flags = CV_HAAR_FIND_BIGGEST_OBJECT;
  for (int i=0; i<cascades.size(); i++) {
    vector<Rect> faces;  
    cascades.at(i)->detectMultiScale(frame, faces,
				     1.1, nearobjs, flags,
				     *minsz );
    rfaces.insert(rfaces.end(), faces.begin(), faces.end());
  }

  if (debug) {
    for (int i=0; i<rfaces.size(); i++) {
      Mat tmp = frame; /* Make a copy of the frame */
      Point center;
      int radius;
      center.x = cvRound((rfaces.at(i).x + rfaces.at(i).width*0.5));
      center.y = cvRound((rfaces.at(i).y + rfaces.at(i).height*0.5));
      radius = cvRound((rfaces.at(i).width + rfaces.at(i).height)*0.25);
      circle( tmp, center, radius, CV_RGB(0,255,0), 3, 8, 0 );
      cv::imshow(wname, frame);
    }
  }

  /* Now, let's call lua handlers, if any */
  for (i=0; i<simple_handlers.size(); i++) {
    lua_getglobal(L, simple_handlers.at(i).data());  /* function to be called */
    lua_pushnumber(L, rfaces.size());   /* push 1st argument */
    if (lua_pcall(L, 1, 0, 0) != 0)
      {
	printf("error running function `%s': %s\n",
	       simple_handlers.at(i).data(), lua_tostring(L, -1));
	assert(0);
      }
  } 
  return rfaces;
}


