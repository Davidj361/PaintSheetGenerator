#include "legend.h"
#include "helper.h"

using namespace std;
using namespace cv;

using uint = unsigned int;
using uchar = unsigned char;

Legend::Legend(Mat& edges, Mat& quantized, const vector<Segment>& segments) : segments(segments), img(edges.clone()), quantized(quantized.clone()) {
	for (size_t i = 0; i < segments.size(); i++) {
		Segment s = segments[i];
		if (findColour(s.getColour()) == -1) {
			colours.push_back(s);
		}
	}
}

void Legend::createLegend(Mat& dst) {
	// Put numbers on the actual image
	for (size_t i = 0; i < segments.size(); i++) {
		int number = findColour(segments[i].getColour());
		if (number == -1)
			throw runtime_error("createLegend broke");

		//number positioning
		int baseline = 0;
		Size textSize = getTextSize(to_string(number + 1), Fontface, 1.0, 2, &baseline);

		// Get proper spot for placing the number
		Point p = findTextSpot(segments[i], textSize);
		// Segment too small for text?
		// cout << p.x << " " << p.y << endl;
		if (p.x == -1 || p.y == -1)
			continue;

		// draw the box
		Rect r = Rect(p.x, p.y, textSize.width, -textSize.height);
		rectangle(img, r.tl(), r.br(), Scalar(0, 0, 255));

		putText(img, to_string(number + 1), p, Fontface, 1.0, Scalar::all(255), 2);
		putText(img, to_string(number + 1), p, Fontface, 1.0, Scalar::all(0), 1);
		// also need to update quantized so no overlapping numbers
		putText(quantized, to_string(number + 1), p, Fontface, 1.0, Scalar::all(255), 2);
		putText(quantized, to_string(number + 1), p, Fontface, 1.0, Scalar::all(0), 1);
	}

	//add black border around img
	copyMakeBorder(img, dst, 1, 1, 1, 1, BORDER_CONSTANT, Scalar::all(0));

	//add inital padding to bottom and right side of img
	copyMakeBorder(dst, dst, 10, 10, 10, 42, BORDER_CONSTANT, Scalar::all(255));


	//point pt1 is rectangle top left corner
	//point pt2 is rectangle bottom right corner
	//with 20 pixel spacing between subsuqent rectangles vertically
	Point pt1 = Point(img.cols + 20, 10);
	Point pt2 = Point(pt1.x + 10, pt1.y + 10);
	Point text;

	// Create the legend
	for (size_t i = 0; i < colours.size(); i++) {

		//added legend entries to right once they reach the height of the orginal img
		//and also extend the img by the needed amount
		if (pt1.y > img.rows) {
			copyMakeBorder(dst, dst, 0, 0, 0, 30, BORDER_CONSTANT, Scalar::all(255));
			pt1 = Point(text.x + 20, 10);
			pt2 = Point(pt1.x + 10, pt1.y + 10);
		}

		rectangle(dst, pt1, pt2, Scalar(colours[i].getColour()), -1);
		rectangle(dst, pt1, pt2, Scalar::all(0), 1);
		text.x = pt1.x + 10;
		text.y = pt2.y;
		putText(dst, to_string(i + 1), text, Fontface, 0.5, Scalar::all(0));

		pt1.y += 20;
		pt2.y += 20;
	}
}

int Legend::findColour(const Scalar& s) const {
	for (size_t i = 0; i < colours.size(); i++) {
		Scalar s2 = colours[i].getColour();
		if (isSameScalar(s, s2)) {
			return i;
		}
	}
	return -1;
}

bool Legend::isSameScalar(const Scalar& s, const Scalar& s2) const {
	if (s[0] == s2[0] && s[1] == s2[1] && s[2] == s2[2]) {
		return true;
	}
	return false;
}

bool Legend::validTextArea(const Point& p, const Size& textSize, const Scalar& colour) const {
	// Check area of textSize if all the same pixel colour

	int yLimit = p.y + textSize.height;
	int xLimit = p.x + textSize.width;
	for (int row = p.y; row < yLimit; row++) {
		for (int col = p.x; col < xLimit; col++) {
			// Boundary check
			Scalar c(quantized.at<Vec3b>(row, col));
			if (col < 0 || row < 0 || col > quantized.cols || row > quantized.rows ||
				!isSameScalar(c, colour)) {
				return false;
			}
		}
	}
	return true;
}

Point Legend::findTextSpot(const Segment s, const Size& textSize) const {
	// Set as an invalid point
	// That way if we any point in the segment is too small for text then don't draw it
	Point ret(-1, -1);
	Scalar colour = Vec3b(s.getColour());

	// Go through all pixels of the segment
	for (auto p : s.getPoints()) {
		if (validTextArea(p, textSize, colour)) {
			ret.x = p.x;
			ret.y = p.y + textSize.height;
			break;
		}
	}
	return ret;
}
