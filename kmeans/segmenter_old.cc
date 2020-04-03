#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

using uint = unsigned int;

/*
Attempts to segment an image using only edge detection
params:
	img: input image
	result: output image
*/
int edge_only(Mat img, Mat &result) {
	Mat blur;
	int lower_t, upper_t, kernal_width;

	lower_t = 10;
	upper_t = 200;
	
	kernal_width = 7;

	GaussianBlur(img, blur, Size(kernal_width, kernal_width),0);
	Canny(blur, result, lower_t, upper_t);
	return 0;
}

/*
Uses kmeans cluster to improve segmentation when finding edges
params: 
	img: input image
	result: output image
*/
int kmeans_then_edges(Mat img, Mat& result) {
	Mat img_data, labels, centers;

	//TODO: calculate k based on how many distinct colours there are
	int k = 4;

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
	vector<vector<Vec3f>> colour_segments(k);
	
	Vec3f* pixel = img_data.ptr<Vec3f>();
	for (int i = 0; i < img_data.rows; i++) {
		int center_label = labels.at<int>(i);
		colour_segments[center_label][i] = centers.at<Vec3f>(center_label);
		
	}

	Mat result_temp = img_data.reshape(3, img.rows);
	result_temp.convertTo(result_temp, CV_8U);
	imshow("kmeans", result_temp);

	edge_only(result_temp, result);
	return 0;
	
	
}
int seperate_components(vector<Vec3f> centers, Mat image, Mat& result) {

}
int main() {
	Mat img, result_edge, result_kmeans;
	img = imread("tree1_ca.jpg", 1);
	imshow("input", img);

	edge_only(img, result_edge);

	if (1) {
		kmeans_then_edges(img, result_kmeans);
		imshow("result kmeans", result_kmeans);
	}

	imshow("result edge only", result_edge);
	waitKey(0);

	return 0;
}
