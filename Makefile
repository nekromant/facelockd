all:
	gcc facedetect.cpp `pkg-config --libs opencv` -lstdc++ -lm
