#ifndef PAINTSHEET_H
#define PAINTSHEET_H

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
#include "legend.h"
#include "borders.h"


using namespace std;
using namespace cv;


class PaintSheet {
public:
	friend class Window;
	
	PaintSheet(Mat& input, int k, bool dial8 = true, bool drawBoxes=false);

	void showOrig();
	void showQuantized();
	void showProduct();
private:
	Segmenter segmenter;
	vector<Segment> segments;
	string title;
	Mat orig, quantized, quantizedNoEdges, product, kmeans;
	int k;
	bool drawBoxes, dial8;

	void quantize(Mat& input);
	void dilation(int size, int numTimes);
};

#endif
