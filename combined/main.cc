#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "../legend/legend.h"

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
	const size_t k = static_cast<size_t>(stoi(argv[2]));

	Segmenter segmenter = Segmenter(img, k);
	vector<Segment> segments = segmenter.getSegments();
	// Mat imageWithCenters = img.clone();
	// for (Segment segment : segments) {
	// 	display(segment.asMat(img.size()));
	// 	drawMarker(imageWithCenters, segment.getCenter(), Scalar(0, 0, 255));
	// }
	// display(imageWithCenters);

	Mat dilation_dst, regionsWithNumbers, result_edge, legendImg;
	edge_only(img, result_edge);
	Dilation(result_edge, dilation_dst, 3, 2);
	Legend legend(dilation_dst, segments);
	legend.createLegend(legendImg);
	display(legendImg);

	return 0;
}
