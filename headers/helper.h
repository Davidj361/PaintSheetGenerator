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


// Helper functions
#ifndef HELPER_H
#define HELPER_H

#include "legend.h"

/*
  Attempts to segment an image using only edge detection
  params:
  img: input image
  result: output image
*/
int edge_only(Mat img, Mat& result);

void getImage(Mat& in, const char* s);

void display(const Mat& image, const string& title="TEST");

#endif
