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
	Legend(Mat& in, const vector<Segment>& segments) : segments(segments) {
		cvtColor(in, img, COLOR_GRAY2RGB);
		for (size_t i = 0; i < segments.size(); i++) {
			Segment s = segments[i];
			if (findColour(s.getColour()) == -1) {
				colours.push_back(s);
			}
		}
	}

	void createLegend(Mat& dst) {
		// Put numbers on the actual image
		for (size_t i = 0; i < segments.size(); i++) {
			int number = findColour(segments[i].getColour());
			if (number == -1)
				throw runtime_error("createLegend broke");

			//number positioning
			int baseline = 0;
			Size textSize = getTextSize(to_string(number + 1), Fontface, 1.0, 2, &baseline);
			
			// draw the box
			Rect r = Rect(segments[i].getCenter().x, segments[i].getCenter().y, textSize.width, -textSize.height);
			rectangle(img, r.tl(), r.br(), Scalar(0, 0, 255));

			putText(img, to_string(number + 1), segments[i].getCenter(), Fontface, 1.0, Scalar::all(255), 2);
			putText(img, to_string(number + 1), segments[i].getCenter(), Fontface, 1.0, Scalar::all(0), 1);
		}

		//add black border around img
		copyMakeBorder(img, dst, 1, 1, 1, 1, BORDER_CONSTANT, Scalar::all(0));

		//add inital padding to bottom and right side of img
		copyMakeBorder(dst, dst, 10, 10, 10, 42, BORDER_CONSTANT, Scalar::all(255));

		//point pt1 is rectangle top left corner
		//point pt2 is rectangle bottom right corner
		//with 20 pixel spacing between subsuqent rectangles vertically
		Point pt1 = Point(img.cols + 20, 10);
		Point pt2 = Point(pt1.x + 10, pt1.y + 10);
		Point text;

		// Create the legend
		for (size_t i = 0; i < colours.size(); i++) {

			//added legend entries to right once they reach the height of the orginal img
			//and also extend the img by the needed amount 
			//(rectangle width + text width + extra padding) 10 + 12 + 20
			if (pt1.y > img.rows) {
				cout << "max height reached!\ncreating new legend column..." << endl;
				copyMakeBorder(dst, dst, 0, 0, 0, 30, BORDER_CONSTANT, Scalar::all(255));
				pt1 = Point(text.x + 20, 10);
				pt2 = Point(pt1.x + 10, pt1.y + 10);
			}

			rectangle(dst, pt1, pt2, Scalar(colours[i].getColour()), -1);
			rectangle(dst, pt1, pt2, Scalar::all(0), 1);
			text.x = pt1.x + 10;
			text.y = pt2.y;
			putText(dst, to_string(i + 1), text, Fontface, 0.5, Scalar::all(0));

			pt1.y += 20;
			pt2.y += 20;
		}
	}

private:
	Mat img;
	vector<Segment> segments;
	vector<Segment> colours;
	int Fontface = FONT_HERSHEY_DUPLEX;

	int findColour(const Scalar& s) const {
		for (size_t i = 0; i < colours.size(); i++) {
			Scalar s2 = colours[i].getColour();
			if (isSameScalar(s, s2)) {
				return i;
			}
		}
		return -1;
	}

	bool isSameScalar(const Scalar& s, const Scalar& s2) const {
		if (s[0] == s2[0] && s[1] == s2[1] && s[2] == s2[2]) {
			return true;
		}
		return false;
	}
};


/*
  Attempts to segment an image using only edge detection
  params:
  img: input image
  result: output image
*/
int edge_only(Mat img, Mat& result) {
	Mat blur;
	int lower_t, upper_t, kernal_width;

	lower_t = 10;
	upper_t = 200;

	kernal_width = 7;

	GaussianBlur(img, blur, Size(kernal_width, kernal_width), 0);
	Canny(blur, result, lower_t, upper_t);
	return 0;
}

//Dilation functin to increase thicken the edges
void Dilation(Mat in, Mat& dilation_dst, int size, int numTimes) {
	//create kernal for dilation
	Mat element = getStructuringElement(MORPH_RECT, Size(size, size));

	/// Apply the dilation operation "n" times 
	for (int i = 0; i < numTimes; i++) {
		if (i == 0) {
			dilate(in, dilation_dst, element);
		}
		else {
			dilate(dilation_dst, dilation_dst, element);

		}
	}
	bitwise_not(dilation_dst, dilation_dst);
}

#endif
