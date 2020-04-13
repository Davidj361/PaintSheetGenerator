#ifndef COLOURBOOK_H
#define COLOURBOOK_H

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


class ColourBook {
public:
	ColourBook(Mat& edges, int k, bool drawBoxes=false);

	void showOrig();
	void showQuantized();
	void showProduct();
private:
	Segmenter segmenter;
	vector<Segment> segments;
	string title;
	Mat orig, quantized, product, kmeans;
	const int k;
	const bool drawBoxes;

	void quantize();
	void dilation(int size, int numTimes);
};

#endif
