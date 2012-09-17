#ifndef __CVINPUTSTREM_H
#define __CVINPUTSTREM_H

#include "detectable.hpp"

class cvInputStream : inputStream {
  CvCapture* capture;
  vector<detectable*> detectables;
  String wname;
public:
  cvInputStream(lua_State *L, String instance);
  void processNextFrame(lua_State* L);
};

#endif
