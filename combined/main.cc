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

	//centers = centers.reshape(3);
	//img_data = img_data.reshape(3);

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
		// display(dst);
	}
	
}

//calculates average colour within a contour by creating a mask for each region
//then adds colours to a list
void getAvergeColour(Mat in, vector<Mat> contours, vector<Scalar>& averageColours) {
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
		// display(dst);
	}
}

void findCenters(Mat in, vector<Mat> contours, vector<Scalar> averageColours, vector<Point>& points) {
	
	Mat dst; 
	cvtColor(in, dst, COLOR_GRAY2RGB);

	for (size_t i = 0; i < contours.size(); i++) {
		// finds center point of current contour
		Moments m = moments(contours[i], true);
		Point p(m.m10 / m.m00, m.m01 / m.m00);

		//add to list of center points
		points.push_back(p);

		//draw coloured circle and corresponding number
		//circle(in, p, 10, averageColours[i], -1);
		putText(dst, to_string(i+1), p, FONT_HERSHEY_DUPLEX, 1.0, averageColours[i]);

	}

	// display(dst);


}

int main(int argc, char** argv) {
	if (argc != 3) {
		cout << "usage: <prog> <image> <k>" << endl;
		return -1;
	}
	Mat img, greyImg, result_edge, result_kmeans;
	vector<Mat> contours;
	vector<Scalar> averageColours;
	vector<Point> points;

	getImage(img, argv[1]);
    const int k = atoi(argv[2]);
	// edge_only(img, result_edge);
	myKmeans(img, result_kmeans, k);
	display(result_kmeans);
	// cout << result_edge.type() << endl;
	// cout << result_edge.depth() << endl;
	// cout << result_edge.channels() << endl;
	// display(result_edge);
	// cvtColor(result_edge, greyImg, COLOR_BGR2GRAY, 1);
	// getRegions(result_edge, contours);
	// getAvergeColour(img, contours, averageColours);
	// findCenters(result_edge, contours, averageColours, points);

	return 0;
}
