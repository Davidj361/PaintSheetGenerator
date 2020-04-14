#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "colourbook.h"


using namespace std;
using namespace cv;


class Window {
public:
	Window(Mat& input);

	void run();
private:
	string title, trackbarName;
	Mat img;
	int k, kmax;
	enum windowType { ORIG, QUANT, PROD } type;
	ColourBook cb;

	static void onTrackbar(int, void*);
	void realTrack(int i);
	static void onMouse(int event, int x, int y, int flags, void* userdata);
	static void onButtonOrig(int i, void* ptr);
	static void onButtonQuant(int i, void* ptr);
	static void onButtonProd(int i, void* ptr);
	static void onButtonRun(int i, void* ptr);
	void draw(int i, bool recalculate=false);
};

#endif
