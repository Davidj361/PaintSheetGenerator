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


#ifndef BORDERS_H
#define BORDERS_H

#include <iostream>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "../headers/segmenter.h"

using namespace std;
using namespace cv;


class Borders {
public:
	static Mat create(Mat& img, vector<Segment>& segments) {
		// Black and white image of the same dimensions as img
		Mat mat = Mat(img.size(), CV_8U, Scalar(0));
		for (auto s: segments) {
			mat += getEdges(img, s);
		}
		cvtColor(mat, mat, COLOR_GRAY2RGB);
		return mat;
	}

private:
	static Mat getEdges(const Mat& img, Segment& s) {
		int lower_t, upper_t, kernal_width;
		lower_t = 10;
		upper_t = 200;
		kernal_width = 7;
		Mat m = s.asBinaryMat(img.size());
		Mat b, ret;
		GaussianBlur(m, b, Size(kernal_width, kernal_width), 0);
		Canny(b, ret, lower_t, upper_t);
		return ret;
	}
};

#endif
