#ifndef BORDERS_H
#define BORDERS_H

#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "../headers/segmenter.h"

using namespace std;
using namespace cv;


class Borders {
public:
	Mat mat;
	Borders(Mat& img, vector<Segment>& segments) {
		// Black and white image of the same dimensions as img
		mat = Mat(img.size(), CV_8U, Scalar(0));
		for (auto s: segments) {
			mat += getEdges(img, s);
		}
		cvtColor(mat, mat, COLOR_GRAY2RGB);
	}

private:
	Mat getEdges(const Mat& img, Segment& s) {
		int lower_t, upper_t, kernal_width;
		lower_t = 10;
		upper_t = 200;
		kernal_width = 7;
		Mat m = s.asBinaryMat(img.size());
		Mat b, ret;
		GaussianBlur(m, b, Size(kernal_width, kernal_width), 0);
		Canny(b, ret, lower_t, upper_t);
		return ret;
	}
};

#endif
