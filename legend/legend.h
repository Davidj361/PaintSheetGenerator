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
#include "../kmeans/segmenter.h"

using namespace std;
using namespace cv;

using uint = unsigned int;
using uchar = unsigned char;


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

			// Get proper spot for placing the number
			Point p = findTextSpot(segments[i], textSize);
			// Segment too small for text?
			// cout << p.x << " " << p.y << endl;
			if (p.x == -1 || p.y == -1)
				continue;
			
			// draw the box
			Rect r = Rect(p.x, p.y, textSize.width, -textSize.height);
			rectangle(img, r.tl(), r.br(), Scalar(0, 0, 255));

			Mat templ = Mat(textSize.height, textSize.width, CV_8UC3);
			templ = colours[i].getColour();
			Mat res;
			matchTemplate(img, templ,res, TM_CCOEFF_NORMED);

			putText(img, to_string(number + 1), p, Fontface, 1.0, Scalar::all(255), 2);
			putText(img, to_string(number + 1), p, Fontface, 1.0, Scalar::all(0), 1);
		}

		//add padding to bottom and right side of img
		copyMakeBorder(img, dst, 0, img.rows, 0, img.cols, BORDER_CONSTANT, Scalar::all(255));

		//point pt1 is rectangle top left corner
		//point pt2 is rectangle bottom right corner
		//with 20 pixel spacing between subsuqent rectangles vertically
		Point pt1 = Point(img.cols + 10, 10);
		Point pt2 = Point(pt1.x + 10, pt1.y + 10);
		Point text;

		// Create the legend
		for (size_t i = 0; i < colours.size(); i++) {

			if (pt1.y > img.rows) {
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

	bool validTextArea(Point& p, Size& textSize, const Scalar& colour) const {
		// Check area of textSize if all the same pixel colour
		bool validArea = true;
		for (int col = p.x - textSize.width; col < p.x + textSize.width; col++) {
			for (int row = p.y - textSize.height; row < p.y + textSize.height; row++) {
				// Boundary check
				if (col < 0 || row < 0 || col > img.cols || row > img.rows ||
					!isSameScalar(img.at<uchar>(col, row), colour) )
					validArea = false;
				break;
			}
		}
		return validArea;
	}

	Point findTextSpot(const Segment s, const Size& textSize) const {
		// Set as an invalid point
		// That way if we any point in the segment is too small for text then don't draw it
		Point ret(-1, -1);
		uint width = textSize.width / 2;
		uint height = textSize.height / 2;
		Size size(width, height);
		Scalar colour = s.getColour();

		// Go through all pixels of the segment
		for (auto p: s.getPoints())
			if (validTextArea(p, size, colour)) {
				ret.x = p.x - width;
				ret.y = p.y - height;
			}
		return ret;
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

}

#endif
