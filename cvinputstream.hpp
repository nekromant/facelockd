#ifndef __CVINPUTSTREM_H
#define __CVINPUTSTREM_H

class cvInputStream : inputStream {
  CvCapture* capture;
  
public:
  int loadConfigFromLua(lua_State *L, String instance);
  Mat getNextFrame();
};

#endif
