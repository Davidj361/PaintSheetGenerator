#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "../headers/window.h"
#include "../headers/helper.h"

using namespace std;
using namespace cv;

using uint = unsigned int;
using uchar = unsigned char;


int main(int argc, char** argv) {
	if (argc != 2) {
		cout << "usage: <prog> <image>" << endl;
		return -1;
	}

	Mat img;
	getImage(img, argv[1]);
	Window window(img);
	window.run();

	return 0;
}
