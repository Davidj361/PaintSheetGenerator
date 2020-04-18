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


#ifndef WINDOW_H
#define WINDOW_H

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
#include "paintsheet.h"


using namespace std;
using namespace cv;


class Window {
public:
	Window(Mat& input);

	void run();
private:
	string title, trackbarName;
	Mat img;
	int k, kmax;
	enum windowType { ORIG, QUANT, QUANT_NOE, PROD } type;
	PaintSheet ps;

	static void onTrackbar(int, void*);
	void realTrack(int i);
	static void onMouse(int event, int x, int y, int flags, void* userdata);
	void showOriginal();
	void showQuantized();
	void showQuantizedNoEdges();
	void showProduct();
	void doRun(bool dial8 = true);
	void doSave();
	void draw(bool recalculate = false, bool dial8 = true );
};

#endif
