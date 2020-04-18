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
#include "../headers/segmenter.h"


using namespace std;
using namespace cv;


const string TITLE = "TEST";

vector<Point> Segment::getPoints() { return this->points; }
vector<Point> Segment::getPoints() const { return this->points; }
Vec3f Segment::getColour() const { return this->colour;  }
Point Segment::getCenter() const { return this->center;  }
void Segment::setCenter(Point point) { this->center = point; }

Segment::Segment(){}
Segment::Segment(Vec3f colour) {
	this->points = vector<Point>();
	this->colour = colour;
}

int Segment::addPoint(Point point) {
	this->points.push_back(point);
	return 0;
}

Mat Segment::asBinaryMat(Size size) {
	Mat image = Mat::zeros(size, CV_8U);;
	for (Point point : this->points) {
		image.at<uchar>(point) = 255;
	}
	return image;
}





Mat Segment::asMat(Size size) {
	Mat image = Mat(size, CV_8UC3, Vec3b(0, 0, 0));
	for (Point point : this->points) {
		image.at<Vec3b>(point) = this->colour;
	}
	return image;
}


Segmenter::Segmenter() {}
Segmenter::Segmenter(Mat image, size_t k) {
	this->image = image;
	this->findSegments(k);
}
vector<Segment> Segmenter::getSegments() { return this->segments; }

//Splits a segment into it's strongly connected components
//Only relevent when multiple segments per colour
int Segmenter::splitSegment(Segment segment, vector<Segment>& splitSegments) {
	Mat binaryImage = Mat::zeros(this->image.size(), CV_8U);
	for (Point point : segment.getPoints()) {
		binaryImage.at<uchar>(point) = 255;
	}

	Mat labels, stats, centroids;
	connectedComponentsWithStats(binaryImage, labels, stats, centroids, 8, CV_32S);

	//Map to store segments since we need to skip background pixels
	map<int, Segment> segmentsMap;
	for (int row = 0; row < labels.rows; row++) {
		for (int col = 0; col < labels.cols; col++) {
			Point point(col, row);
			//Ignore the background
			if (int(binaryImage.at<uchar>(point)) == 255) {
				int label = labels.at<int>(point);
				//Ignore segments < 1% of picture
				if (stats.at<int>(label, 4) > 0.0001*(double(this->image.rows* this->image.cols))) {
					if (segmentsMap.count(label) == 0) {
						Segment newSegment = Segment(segment.getColour());
						newSegment.setCenter(Point(int(centroids.at<double>(label, 0)), int(centroids.at<double>(label, 1))));
						segmentsMap[label] = newSegment;
					}
					segmentsMap[label].addPoint(point);
				}
				
			}
		}
	}

	for (auto element : segmentsMap) {
		splitSegments.push_back(element.second);
	}
	
	return 0;
}

/*
Uses k-means to seperate image into k colours
*/
int Segmenter::segmentColours(size_t k) {
	Mat img_data, labels, centers;
	//this->segments = vector<Segment>(k);

	this->image.convertTo(img_data, CV_32F);
	img_data = img_data.reshape(1, this->image.rows * this->image.cols);

	kmeans(img_data,
		k,
		labels,
		TermCriteria(TermCriteria::Type::MAX_ITER + TermCriteria::Type::EPS, 10, 1.0),
		10,
		KMEANS_RANDOM_CENTERS,
		centers);

	centers = centers.reshape(3, centers.rows);
	img_data = img_data.reshape(3, img_data.rows);

	//create segment for each colour
	for (int i = 0; i < centers.rows; i++) {
		Vec3f colour = centers.at<Vec3f>(i);
		this->segments.push_back(Segment(colour));
		this->colours.push_back(colour);
	}

	assert(this->image.rows * this->image.cols == labels.rows);
	int i = 0;
	for (int row = 0; row < this->image.size().height; row++) {
		for (int col = 0; col < this->image.size().width; col++) {
			int center_label = labels.at<int>(i);
			this->segments[center_label].addPoint(Point(col, row));
			i++;
		}
	}
	return 0;
}



int Segmenter::findSegments(size_t k) {
	assert(!this->image.empty());
	
	segmentColours(k);
	assert(this->segments.size() == k);

	vector<Segment> splitSegments;
	for (Segment segment : this->segments) {
		vector<Segment> newSegments;
		splitSegment(segment, newSegments);
		splitSegments.insert(splitSegments.end(), newSegments.begin(), newSegments.end());
	}

	this->segments = splitSegments;

	return 0;
}

int Segmenter::getKmeansImage(Mat& result) {
	for (Segment segment : this->segments) {
		assert(result.size() == this->image.size());
		result += segment.asMat(this->image.size());
	}
	return 0;
}


