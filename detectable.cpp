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


detectable::detectable(lua_State* L, String instance){
  printf("Loading a detectable object: %s\n", instance.data());
  lua_getglobal(L,instance.data());
  assert(lua_istable(L, -1));
  
  get_boolean_field(enabled);
  get_boolean_field(debug);
  get_int_field(nearobjs);

  /* Now read out the cascades */
  lua_pushstring(L, "cascades");
  lua_gettable(L, -2);
  int i = 1;
  while (1)
    {
      const char *s;  
      get_str_member(s,i++, lua_pop(L, 1); break; );
      printf("Loading cascade: %s\n", s);
      CascadeClassifier *cascade = new CascadeClassifier;
      assert(cascade->load(s));
      cascades.push_back(cascade);
    }
  lua_pop(L,1);
  int w,h;
  lua_pushstring(L, "maxsize");
  lua_gettable(L, -2);
  assert(lua_istable(L, -1));
  get_int_member(w,1, assert(0););
  get_int_member(h,2, assert(0););
  lua_pop(L,1);
  maxsz = new Size(w,h);
  printf("Detectable: %s (%dx%d); nearobjs %d; debug: %d; enabled: %d\n", 
	 instance.data(), w, h, nearobjs, debug, enabled);
  lua_pop(L,1);
  
}
