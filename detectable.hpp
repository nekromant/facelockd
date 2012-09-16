#ifndef __DETECTABLE_H
#define __DETECTABLE_H

class detectable {
public:
  detectable(lua_State* L, String instance);
  Rect detect(Mat& frame);
  int enabled, debug, nearobjs;
  Size *maxsz;
  vector<CascadeClassifier*> cascades;
};

#endif
