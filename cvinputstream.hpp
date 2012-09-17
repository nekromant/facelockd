#ifndef __CVINPUTSTREM_H
#define __CVINPUTSTREM_H

#include "detectable.hpp"

class cvInputStream : inputStream {
  CvCapture* capture;
  vector<detectable*> detectables;
  String wname, wpname;
  int debug, cprofile;
  int framedelay;
public:
  cvInputStream(lua_State *L, String instance);
  void processNextFrame(lua_State* L);
};

#endif
