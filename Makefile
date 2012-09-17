sources=facedetect.cpp cvinputstream.cpp detectable.cpp

all:
	gcc $(sources) -g `pkg-config --libs opencv` -lstdc++ -lm -llua -o facelockd 

