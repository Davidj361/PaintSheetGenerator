#ifndef LEGEND_H
#define LEGEND_H

#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "segmenter.h"

using namespace std;
using namespace cv;

class Legend {
public:
	Legend(Mat& edges, Mat& quantized, const vector<Segment>& segments);
	void createLegend(Mat& dst);

private:
	Mat img, quantized;
	vector<Segment> segments;
	vector<Segment> colours;
	int Fontface = FONT_HERSHEY_DUPLEX;

	int findColour(const Scalar& s) const;
	bool isSameScalar(const Scalar& s, const Scalar& s2) const;

	bool validTextArea(const Point& p, const Size& textSize, const Scalar& colour) const;

	Point findTextSpot(const Segment s, const Size& textSize) const;
};



#endif
