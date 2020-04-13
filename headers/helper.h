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
