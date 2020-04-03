#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

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
	findContours(in, contours, hier, RETR_TREE, CHAIN_APPROX_NONE);

	//display found regions
	for (uint iter = 0; iter < contours.size(); iter++) {
		Mat dst = Mat::zeros(in.rows, in.cols, CV_8UC3);
		drawContours(dst, contours, iter, Scalar(0, 0, 255), FILLED);
		display(dst);

	}
	
}

//calculates average colour within a contour by creating a mask for each region
//then adds colours to a list
void getAvergeColour(Mat in, vector<Mat> contours, vector<Scalar>& averageColours) {
	

	for (uint iter = 0; iter < contours.size(); iter++) {
		
		//create mask for a contour
		Mat1b mask(in.rows, in.cols, uchar(0));
		drawContours(mask, contours, iter, Scalar::all(255), FILLED);
		//display(mask);

		//calculate average colour value for current contour region
		Scalar average = mean(in, mask);

		//add to list of average colours
		averageColours.push_back(average);

		//display avergae colour of contours
		Mat dst = Mat(in.rows, in.cols, CV_8UC3, Scalar::all(255));
		drawContours(dst, contours, iter, average, FILLED);
		display(dst);
			

	}

}

void findCenters(Mat in, vector<Mat> contours, vector<Scalar> averageColours, vector<Point>& points) {
	
	Mat dst; 
	cvtColor(in, dst, COLOR_GRAY2RGB);

	for (int i = 0; i < contours.size(); i++) {
		// finds center point of current contour
		Moments m = moments(contours[i], true);
		Point p(m.m10 / m.m00, m.m01 / m.m00);

		//add to list of center points
		points.push_back(p);

		//draw coloured circle and corresponding number
		//circle(in, p, 10, averageColours[i], -1);
		putText(dst, to_string(i+1), p, FONT_HERSHEY_DUPLEX, 1.0, averageColours[i]);

	}

	display(dst);


}

int main(int argc, char** argv) {
	if (argc != 2) {
		cout << "usage: <prog> <image>" << endl;
		return -1;
	}
	Mat img, greyImg;
	vector<Mat> contours;
	vector<Scalar> averageColours;
	vector<Point> points;

	getImage(img, argv[1]);
	cvtColor(img, greyImg, COLOR_BGR2GRAY, 1);
	getRegions(greyImg, contours);

	getAvergeColour(img, contours, averageColours);

	findCenters(greyImg, contours, averageColours, points);


	return 0;
}