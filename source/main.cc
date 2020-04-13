#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "../headers/colourbook.h"
#include "../headers/helper.h"

using namespace std;
using namespace cv;

using uint = unsigned int;
using uchar = unsigned char;


int main(int argc, char** argv) {
	if (argc != 3) {
		cout << "usage: <prog> <image> <k>" << endl;
		return -1;
	}

	Mat img;
	getImage(img, argv[1]);
	const size_t k = static_cast<size_t>(stoi(argv[2]));

	ColourBook cb(img, k, true);
	cb.showOrig();
	cb.showQuantized();
	cb.showProduct();

	return 0;
}
