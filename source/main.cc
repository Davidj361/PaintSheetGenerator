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
#include "../headers/helper.h"

using namespace std;
using namespace cv;

using uint = unsigned int;
using uchar = unsigned char;

Mat generateQuantized(Mat image) {
	size_t k = 3;
	Segmenter segmenter = Segmenter(image, k);
	vector<Segment> segments = segmenter.getSegments();

	Mat kmeans = Mat::zeros(image.size(), CV_8UC3);
	segmenter.getKmeansImage(kmeans);

	Borders borders(image, segments);
	Mat result_edge = borders.mat;

	Mat dilation_dst;
	Dilation(result_edge, dilation_dst, 3, 2);
	
	Mat dilation_dst_C3(dilation_dst);

	Mat overlay_dil(image.size(), CV_8UC3);
	subtract(kmeans, ~dilation_dst_C3, overlay_dil);

	Mat result_edge_C3(result_edge);

	Mat overlay(image.size(), CV_8UC3);
	subtract(kmeans, result_edge_C3, overlay);

	return overlay;
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

	Mat kmeans = Mat::zeros(img.size(), CV_8UC3);
	segmenter.getKmeansImage(kmeans);

	Mat dilation_dst, regionsWithNumbers, result_edge, legendImg;

	Borders borders(img, segments);
	result_edge = borders.mat;
	display(result_edge);

	Dilation(result_edge, dilation_dst, 3, 2);
	Mat quantized =	generateQuantized(img);
	Legend legend(dilation_dst, quantized, segments);
	// Legend legend(quantized, quantized, segments);
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
