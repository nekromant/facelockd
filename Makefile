sources=facedetect.cpp cvinputstream.cpp detectable.cpp

all:
	gcc $(sources) `pkg-config --libs opencv` -lstdc++ -lm -llua -o facelockd 

