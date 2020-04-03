#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "../legend/legend.h"
#include "../kmeans/segmenter.h"

using namespace std;
using namespace cv;

using uint = unsigned int;
using uchar = unsigned char;
const string TITLE = "TEST";


void getImage(Mat& in, const char* s) {
	in = imread(s, 1);
	if (!in.data) {
		throw runtime_error("No input file");
	}
}


void display(const Mat& image) {
	//namedWindow("Display Image", WINDOW_AUTOSIZE );
	imshow(TITLE, image);
	// If Escape is hit, close
	while (true) {
		int k = waitKey(10);
		if (k == 27) break;
	}
}


int main(int argc, char** argv) {
	if (argc != 3) {
		cout << "usage: <prog> <image> <k>" << endl;
		return -1;
	}
	Mat img;

	getImage(img, argv[1]);
	Mat imageWithCenters = img.clone();
	Segmenter segmenter = Segmenter(img);
	vector<Segment> segments = segmenter.getSegments();
	int count = 0;
	for (Segment segment : segments) {
		drawMarker(imageWithCenters, segment.getCenter(), Scalar(0, 0, 255));
		count++;
	}

	Mat dilation_dst, regionsWithNumbers, legendImg, result_edge;
	vector<Mat> contours;
	vector<Scalar> averageColours;
	vector<Point> points;
	edge_only(img, result_edge);
	Dilation(result_edge, dilation_dst, 3, 2);
	getRegions(dilation_dst, contours);
	getAverageColour(img, contours, averageColours);
	findCenters(dilation_dst, contours, points, regionsWithNumbers);
	createLegend(regionsWithNumbers, legendImg, averageColours);
	display(legendImg);

	return 0;
}
