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
	enum windowType { ORIG, QUANT, QUANT_NOE, PROD } type;
	ColourBook cb;

	static void onTrackbar(int, void*);
	void realTrack(int i);
	static void onMouse(int event, int x, int y, int flags, void* userdata);
	void showOriginal();
	void showQuantized();
	void showQuantizedNoEdges();
	void showProduct();
	void doRun();
	void doSave();
	void draw(bool recalculate = false);
};

#endif
