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


#ifndef LEGEND_H
#define LEGEND_H

#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "segmenter.h"

using namespace std;
using namespace cv;

class Legend {
public:
	Legend(Mat& edges, Mat& quantized, const vector<Segment>& segments);
	void createLegend(Mat& dst, bool drawBoxes=false);

private:
	Mat img, quantized;
	vector<Segment> segments;
	vector<Segment> colours;
	int Fontface = FONT_HERSHEY_DUPLEX;

	int findColour(const Scalar& s) const;
	bool isSameScalar(const Scalar& s, const Scalar& s2) const;

	bool validTextArea(const Point& p, const Size& textSize, const Scalar& colour) const;

	Point findTextSpot(const Segment s, const Size& textSize) const;
};



#endif
