/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!! Programmed by David Jatczak !!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

using uint = unsigned int;
const string TITLE = "TEST";


void getImage( Mat& in, char* s ) {
    in = imread( s, 1 );
    if ( !in.data ) {
		throw runtime_error("No input file");
    }
}


void display(const Mat& image) {
    //namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow(TITLE, image);
	// If Escape is hit, close
	while (true) {
		int k = waitKey(10);
		if (k==27) break;
	}
}


int main(int argc, char** argv ) {
    if ( argc != 2 ) {
		cout << "usage: <prog> <image>" << endl;
        return -1;
    }
	Mat img;
	getImage( img, argv[1] );
	display(img);
    return 0;
}
