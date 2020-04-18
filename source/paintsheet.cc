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


#include "../headers/paintsheet.h"
#include "../headers/helper.h"

using namespace std;
using namespace cv;


PaintSheet::PaintSheet(Mat& input, int k, string& title, bool dial8, bool drawBoxes) : orig(input), k(k), title(title), drawBoxes(drawBoxes), dial8(dial8) {
	this->segmenter = Segmenter(orig, k);
	this->segments = segmenter.getSegments();

	kmeans = Mat::zeros(orig.size(), CV_8UC3);
	segmenter.getKmeansImage(kmeans);

	// Create black and white image of edges, and thicken the lines
	quantized = Borders::create(orig, segments);
	product = quantized.clone();

	if (dial8)
		dilation(3, 2);
	//invert product so borders are black and bg is white

	// Create a version with colours already filled in
	quantizedNoEdges = Mat::zeros(orig.size(), CV_8UC3);
	quantize(quantized);
	quantize(quantizedNoEdges);
	product = ~product;

	Legend legend(product, quantized, segments);
	legend.createLegend(product, drawBoxes);

	legend = Legend(quantized, quantized, segments);
	legend.createLegend(quantized, drawBoxes);

	legend = Legend(quantizedNoEdges, quantizedNoEdges, segments);
	legend.createLegend(quantizedNoEdges, drawBoxes);
}

void PaintSheet::quantize(Mat& input) {
	Mat overlay_dil(orig.size(), CV_8UC3);
	subtract(kmeans, ~input, overlay_dil);

	Mat overlay(orig.size(), CV_8UC3);
	subtract(kmeans, input, overlay);

	input = overlay;
}


void PaintSheet::showOrig() {
	display(orig, this->title);
}

void PaintSheet::showQuantized() {
	display(quantized, this->title);
}

void PaintSheet::showProduct() {
	display(product, this->title);
}


//Dilation functin to increase thicken the edges
void PaintSheet::dilation(int size, int numTimes) {
	//create kernal for dilation
	Mat element = getStructuringElement(MORPH_RECT, Size(size, size));

	/// Apply the dilation operation "n" times 
	for (int i = 0; i < numTimes; i++) {
		dilate(product, product, element);
	}
	quantized = product.clone();
}
