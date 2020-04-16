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
	friend class Window;
	
	ColourBook(Mat& input, int k, bool drawBoxes=false);

	void showOrig();
	void showQuantized();
	void showProduct();
private:
	Segmenter segmenter;
	vector<Segment> segments;
	string title;
	Mat orig, quantized, quantizedNoEdges, product, kmeans;
	int k;
	bool drawBoxes;

	void quantize(Mat& input);
	void dilation(int size, int numTimes);
};

#endif
