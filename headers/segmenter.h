#ifndef SEGMENTER_H
#define SEGMENTER_H

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


/*
Class: Segment

*/
class Segment {
public:
	Segment();
	Segment(Vec3f);
	int addPoint(Point);
	vector<Point> getPoints();
	Vec3f getColour() const;
	Point getCenter() const;
	void setCenter(Point);
	Mat asBinaryMat(Size);
	Mat asMat(Size);
private:
	vector<Point> points;
	Vec3f colour;
	Point center;
};

/*
Class: Segmenter

*/
class Segmenter {
public:
	Segmenter(Mat, size_t);
	Segmenter();
	int findSegments(size_t);
	vector<Segment> getSegments();
	int getKmeansImage(Mat&);
private:
	int segmentColours(size_t);
	int splitSegment(Segment, vector<Segment>&);
	vector<Segment> segments;
	vector<Vec3f> colours;
	Mat image;


};


#endif
