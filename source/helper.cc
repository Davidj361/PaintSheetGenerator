#include "../headers/helper.h"

using namespace std;

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


void getImage(Mat& in, const char* s) {
	in = imread(s, 1);
	if (!in.data) {
		throw runtime_error("No input file");
	}
}


void display(const Mat& image, const string& title) {
	imshow(title, image);
	// If Escape is hit, close
	while (true) {
		int k = waitKey(10);
		if (k == 27) break;
	}
}
