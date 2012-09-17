#ifndef __DETECTABLE_H
#define __DETECTABLE_H

class detectable {
public:
  detectable(lua_State* L, String instance);
  vector<Rect> detect(Mat& frame, lua_State* L);
  int enabled, debug, nearobjs;
  int flags;
  Size *minsz;
  vector<CascadeClassifier*> cascades;
  vector<String> simple_handlers;
  vector<String> shape_handlers;
  String wname; /* Debug window name */
  
};

#endif
