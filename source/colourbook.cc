#include "../headers/colourbook.h"
#include "../headers/helper.h"

using namespace std;
using namespace cv;


ColourBook::ColourBook(Mat& input, int k, bool drawBoxes) : orig(input), k(k), drawBoxes(drawBoxes) {
	title = "Colouring Book";

	this->segmenter = Segmenter(orig, k);
	this->segments = segmenter.getSegments();

	kmeans = Mat::zeros(orig.size(), CV_8UC3);
	segmenter.getKmeansImage(kmeans);

	// Create black and white image of edges, and thicken the lines
	quantized = Borders::create(orig, segments);
	product = quantized.clone();
	dilation(3, 2);
	// Create a version with colours already filled in
	quantize();

	Legend legend(product, quantized, segments);
	legend.createLegend(product, drawBoxes);

	legend = Legend(quantized, quantized, segments);
	legend.createLegend(quantized, drawBoxes);
}

void ColourBook::quantize() {
	Mat overlay_dil(orig.size(), CV_8UC3);
	subtract(kmeans, ~quantized, overlay_dil);

	Mat overlay(orig.size(), CV_8UC3);
	subtract(kmeans, quantized, overlay);

	quantized = overlay;
}


void ColourBook::showOrig() {
	display(orig, this->title);
}

void ColourBook::showQuantized() {
	display(quantized, this->title);
}

void ColourBook::showProduct() {
	display(product, this->title);
}


//Dilation functin to increase thicken the edges
void ColourBook::dilation(int size, int numTimes) {
	//create kernal for dilation
	Mat element = getStructuringElement(MORPH_RECT, Size(size, size));

	/// Apply the dilation operation "n" times 
	for (int i = 0; i < numTimes; i++) {
		dilate(product, product, element);
	}
	quantized = product.clone();
}
