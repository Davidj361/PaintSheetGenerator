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

/*
Uses kmeans cluster to improve segmentation when finding edges
params:
	img: input image
	result: output image
*/
int myKmeans(Mat img, Mat& result, const int k) {
	Mat img_data, labels, centers;

	img.convertTo(img_data, CV_32F);
	img_data = img_data.reshape(1, img.rows * img.cols);

	kmeans(img_data,
		k,
		labels,
		TermCriteria(TermCriteria::Type::MAX_ITER + TermCriteria::Type::EPS, 10, 1.0),
		10,
		KMEANS_RANDOM_CENTERS,
		centers);

	centers = centers.reshape(3);
	img_data = img_data.reshape(3);

	/*
	NOTE: This combines the segments into 1 picture but we could just return a list of Mats each element a colour
		  then segment
	*/

	/*
	every pixel has a label for what segment it's in
	we know the centers (colours) of k segements
	let every pixel be the same colour as the center it shares a label with
	*/
	Vec3f* pixel = img_data.ptr<Vec3f>();
	for (int i = 0; i < img_data.rows; i++) {
		int center_label = labels.at<int>(i);
		pixel[i] = centers.at<Vec3f>(center_label);
	}

	Mat result_temp = img_data.reshape(3, img.rows);
	result_temp.convertTo(result_temp, CV_8U);

	result = result_temp;
	return 0;
}


// Returns regions that were divided by edges
void getRegions(Mat& in, vector<Mat>& contours) {
	vector<Mat> ret;
	// Go through all white pixels and
	// for (int col=0; col < in.cols; col++) {
	// 	for (int row=0; row < in.rows; row++) {
	// 		//if (in.at<uchar>(row, col) == 0)
	// 	}
	// }

	//vector<Mat> contours;
	vector<Vec4i> hier;
	threshold(in, in, 125, 255, THRESH_BINARY_INV);
	// findContours(in, contours, hier, RETR_TREE, CHAIN_APPROX_NONE);
	findContours(in, contours, RETR_CCOMP, CHAIN_APPROX_TC89_KCOS);

	//display found regions
	for (uint iter = 0; iter < contours.size(); iter++) {
		Mat dst = Mat::zeros(in.rows, in.cols, CV_8UC3);
		drawContours(dst, contours, iter, Scalar(0, 0, 255), FILLED);
		//display(dst);
	}

}

//calculates average colour within a contour by creating a mask for each region
//then adds colours to a list
void getAverageColour(Mat in, vector<Mat> contours, vector<Scalar>& averageColours) {
	for (size_t iter = 0; iter < contours.size(); iter++) {

		//create mask for a contour
		Mat1b mask(in.rows, in.cols, uchar(0));
		drawContours(mask, contours, iter, Scalar::all(255), FILLED);
		//display(mask);

		//calculate average colour value for current contour region
		Scalar average = mean(in, mask);

		//add to list of average colours
		averageColours.push_back(average);

		Mat dst = Mat(in.rows, in.cols, CV_8UC3, Scalar::all(255));
		drawContours(dst, contours, iter, average, FILLED);
		//display(dst);
	}
}

void findCenters(Mat& in, const vector<Segment>& segments, Mat& regionsWithNumbers) {
	Mat dst;
	cvtColor(in, dst, COLOR_GRAY2RGB);
	for (size_t i = 0; i < segments.size(); i++) {
		putText(dst, to_string(i + 1), segments[i].getCenter(), FONT_HERSHEY_DUPLEX, 1.0, Scalar::all(255),2);
		putText(dst, to_string(i + 1), segments[i].getCenter(), FONT_HERSHEY_DUPLEX, 1.0, Scalar::all(0),1);
	}
	 regionsWithNumbers = dst;
}

/** @function Dilation */
void Dilation(Mat in, Mat& dilation_dst, int size, int numTimes){
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

void createLegend(Mat& in, Mat& dst, const vector<Segment>& segments) {
	//add padding to bottom and right side of img
	copyMakeBorder(in, dst, 0, in.rows, 0, in.cols, BORDER_CONSTANT, Scalar::all(255));

	cout << segments.size() << endl;
	//point pt1 is rectangle top left corner
	//point pt2 is rectangle bottom right corner
	//with 20 pixel spacing between subsuqent rectangles vertically
	Point pt1 = Point(in.cols + 10, 10); 
	Point pt2 = Point(pt1.x + 10, pt1.y + 10);
	Point text;

	for (size_t i = 0; i < segments.size(); i++) {
		rectangle(dst, pt1, pt2, Scalar(segments[i].getColour()), -1);
		rectangle(dst, pt1, pt2, Scalar::all(0), 1);
		text.x = pt1.x + 10;
		text.y = pt2.y;
		putText(dst, to_string(i+1), text, FONT_HERSHEY_DUPLEX, 0.5, Scalar::all(0));

		pt1.y += 20;
		pt2.y += 20;
	}
	

}

#endif
