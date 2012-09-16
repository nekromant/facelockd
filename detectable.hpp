#ifndef __DETECTABLE_H
#define __DETECTABLE_H

class detectable {
public:
  detectable(lua_State* L, String instance);
  vector<Rect> detect(Mat& frame);
  int enabled, debug, nearobjs;
  int flags;
  Size *minsz;
  vector<CascadeClassifier*> cascades;
  vector<String> simple_handlers;
  vector<String> shandlers;

};

#endif
