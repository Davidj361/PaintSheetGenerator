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

int kmeans_then_edges(Mat img, Mat& result, int k) {
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

	centers = centers.reshape(3, centers.rows);
	img_data = img_data.reshape(3, img_data.rows);

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

	// edge_only(result_temp, result);
	return 0;
	
	
}


int main(int argc, char** argv) {
	/*if (argc != 3) {
		cout << "usage: <prog> <image> <k>" << endl;
		return -1;
	}*/
	Mat img;
	getImage(img, argv[1]);
	const size_t k = static_cast<size_t>(stoi(argv[2]));

	Segmenter segmenter = Segmenter(img, k);
	vector<Segment> segments = segmenter.getSegments();
	Mat kmeans = Mat::zeros(img.size(), CV_8UC3);;
	segmenter.getKmeansImage(kmeans);
	display(kmeans);
	Mat result_kmeans;
	kmeans_then_edges(img, result_kmeans, k);
	display(result_kmeans);

	Mat dilation_dst, regionsWithNumbers, result_edge, legendImg;
	edge_only(img, result_edge);
	Dilation(result_edge, dilation_dst, 3, 2);
	Legend legend(dilation_dst, segments);
	legend.createLegend(legendImg);
	display(legendImg);
	Mat imageWithCenters = legendImg.clone();
	for (Segment segment : segments) {
		// display(segment.asMat(img.size()));
		drawMarker(imageWithCenters, segment.getCenter(), Scalar(0, 0, 255));
	}
	display(imageWithCenters);

	return 0;
}
