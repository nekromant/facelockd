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


int fails=0;
int matches=0;
int maxfails=50;
int unlockthrshld=10;
int framedelay = 100;
int capx = 320;
int capy = 240;

static double dtcttime;
static double prptime;

vector<Rect> cascadeClassify(Mat& img, CascadeClassifier& cascade, int flags, Size MinSz){
  vector<Rect> faces;
  double t = (double)cvGetTickCount();
  cascade.detectMultiScale( img, faces,
			    1.1, 2, flags,
			    MinSz );
 t = (double)cvGetTickCount() - t;
 dtcttime=t/((double)cvGetTickFrequency()*1000);
  return faces;
}


/* Make the image haar-friendly */
inline Mat imagePrepare(Mat& img) {
  double t = (double)cvGetTickCount();
  Mat gray(img.rows, img.cols, CV_8UC1);
  cvtColor( img, gray, CV_BGR2GRAY );
  equalizeHist( gray, gray );
  t = (double)cvGetTickCount() - t;
  prptime=t/((double)cvGetTickFrequency()*1000);
  return gray;
}


inline void processMatches(vector<Rect> faces)
{
  int cnt = faces.size();
	  if (cnt) {
	    matches++;
	    if ((fails > maxfails) && (matches > unlockthrshld))
	      {
	      system("facelock unlock");
	      fails=0;
	      }

	    if ((fails < maxfails))
		fails=0;
	  }
	  if (0==cnt) {
	    matches=0;
	    fails++;
	    if (fails == maxfails)
	      system("facelock lock");
	  }
	  printf("Preparation: %0.4g ms ; Detection: %0.4g ms ; fails count: [ %3d / %3d ] \r ", prptime, dtcttime, fails,maxfails);
	  
}

static lua_State *L;


void report_errors(lua_State *L, int status)
{
  if ( status!=0 ) {
    std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
    lua_pop(L, 1); // remove error message
  }
}


int main (int argc, const char** argv) {
  L = lua_open();
  luaL_openlibs(L);
  printf("Loading settings %s\n", argv[1]);
  int s = luaL_loadfile(L, argv[1]);
  printf("Done with result %d\n", s);
  if ( s==0 ) {
    // execute Lua program
    s = lua_pcall(L, 0, LUA_MULTRET, 0);
  }
  report_errors(L, s);
  cvInputStream *cvd = new cvInputStream(L,"frontcam");
  while(1)
    {
      cvd->processNextFrame(L);
    }
}



int _old_main( int argc, const char** argv )
{
  CvCapture* capture = 0;
  Mat frame, frameCopy, image;
  const String scaleOpt = "--scale=";
  size_t scaleOptLen = scaleOpt.length();
  const String cascadeOpt = "--cascade=";
  size_t cascadeOptLen = cascadeOpt.length();
  const String nestedCascadeOpt = "--nested-cascade";
  size_t nestedCascadeOptLen = nestedCascadeOpt.length();
  String inputName;
  CascadeClassifier cascade, cascade2;
  double scale = 1;

  capture = cvCaptureFromCAM(0);
	  if (!cascade.load( argv[1] ))
	    {
	      exit(1);
	    }
if (!cascade2.load( argv[2] ))
	    {
	      exit(1);
	    }
  cvNamedWindow( "result", 1 );
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, capx);//1280); 
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, capy);//960); 
  if (!capture) exit(1);
  while (1)
    {
      IplImage* iplImg = cvQueryFrame( capture );
      frame = iplImg; 
      vector<Rect> faces, faces2;
      Mat frame_opt = imagePrepare(frame);
				      //|CV_HAAR_FIND_BIGGEST_OBJECT
				      //|CV_HAAR_DO_ROUGH_SEARCH
				      //|CV_HAAR_DO_CANNY_PRUNING
      faces = cascadeClassify(frame_opt, cascade, CV_HAAR_FIND_BIGGEST_OBJECT, Size(40,40));
      faces2 = cascadeClassify(frame_opt, cascade2, CV_HAAR_FIND_BIGGEST_OBJECT, Size(40,40));
      printf(" %d / %d | ", faces.size(), faces2.size());
	faces.insert(faces.end(), faces2.begin(), faces2.end());
      processMatches(faces);
      //cv::imshow("result", frame);
      fflush(stdout);
      waitKey( framedelay );
    }
}

