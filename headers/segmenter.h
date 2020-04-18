//  Copyright (C) 2020 David Jatczak <david.j.361@gmail.com>
//  Copyright (C) 2020 Cameron Canning-Potter <cameroncanningpotter@cmail.carleton.ca>
//  Copyright (C) 2020 Pravdeep Deol
//  
//  This file is part of Paint Sheet Generator.
//  
//  Paint Sheet Generator is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  Paint Sheet Generator is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with Paint Sheet Generator.  If not, see <https://www.gnu.org/licenses/>.


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
	vector<Point> getPoints() const;
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
