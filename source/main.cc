#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "../headers/borders.h"
#include "../headers/legend.h"
#include "../headers/segmenter.h"

using namespace std;
using namespace cv;

using uint = unsigned int;
using uchar = unsigned char;
const string TITLE = "TEST";

//Test if borders line up with segments 
int borderValidation(Mat image) {
	size_t k = 3;
	Segmenter segmenter = Segmenter(image, k);
	vector<Segment> segments = segmenter.getSegments();

	Mat kmeans = Mat::zeros(image.size(), CV_8UC3);
	segmenter.getKmeansImage(kmeans);
	imshow("kmeans image", kmeans);

	Borders borders(image, segments);
	Mat result_edge = borders.mat;
	imshow("borders pre dilation", ~result_edge);

	Mat dilation_dst;
	Dilation(result_edge, dilation_dst, 3, 2);
	imshow("borders", dilation_dst);
	
	Mat dilation_dst_C3;
	cvtColor(dilation_dst, dilation_dst_C3, COLOR_GRAY2BGR);

	Mat overlay_dil(image.size(), CV_8UC3);
	subtract(kmeans, ~dilation_dst_C3, overlay_dil);
	imshow("overlay dilation", overlay_dil);

	Mat result_edge_C3;
	cvtColor(result_edge, result_edge_C3, COLOR_GRAY2BGR);

	Mat overlay(image.size(), CV_8UC3);
	subtract(kmeans, result_edge_C3, overlay);
	imshow("overlay", overlay);

	waitKey(0);
	return 0;
}
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

	if (k == -1) {
		borderValidation(img);
		return 0;
	}

	

	Segmenter segmenter = Segmenter(img, k);
	vector<Segment> segments = segmenter.getSegments();

	Mat kmeans = Mat::zeros(img.size(), CV_8UC3);
	segmenter.getKmeansImage(kmeans);

	Mat dilation_dst, regionsWithNumbers, result_edge, legendImg;

	Borders borders(img, segments);
	result_edge = borders.mat;
	display(result_edge);

	Dilation(result_edge, dilation_dst, 3, 2);
	Legend legend(dilation_dst, segments);
	legend.createLegend(legendImg);
	display(legendImg);

	Mat imageWithCenters = legendImg.clone();
	for (Segment segment : segments) {
		// display(segment.asMat(img.size()));
		drawMarker(imageWithCenters, segment.getCenter(), Scalar(0, 0, 255));
	}
	// display(imageWithCenters);

	return 0;
}
