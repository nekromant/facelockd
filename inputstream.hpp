#ifndef __INPUTSTREAM_H
#define __INPUTSTREAM_H

class inputStream {
public: 
  virtual int loadConfigFromLua(lua_State *L, String instance) { };
  virtual Mat getNextFrame() { } ;
};

#endif
