#include <opencv2/highgui.hpp>
#include "../headers/window.h"
#include "../headers/helper.h"

Window::Window(Mat& input) : k(3), kmax(20), img(input), cb(img, k) {
	type = PROD;
	title = "Colouring Book";
	trackbarName = "k-value";
}

void Window::onMouse(int event, int x, int y, int flags, void* userdata) {
}

void Window::onTrackbar(int i, void* ptr) {
	Window* that = (Window*) ptr;
	that->realTrack(i);
}

void Window::realTrack(int i) {
	if (i < 3) {
		k = 3;
		setTrackbarPos(trackbarName, title, k);
		return;
	}
}

void Window::onButtonOrig(int i, void* ptr) {
	Window* that = (Window*) ptr;
	that->type = ORIG;
	that->draw(i);
}

void Window::onButtonQuant(int i, void* ptr) {
	Window* that = (Window*) ptr;
	that->type = QUANT;
	that->draw(i);
}

void Window::onButtonProd(int i, void* ptr) {
	Window* that = (Window*) ptr;
	that->type = PROD;
	that->draw(i);
}

void Window::onButtonRun(int i, void* ptr) {
	Window* that = (Window*) ptr;
	that->draw(i, true);
}

void Window::draw(int i, bool recalculate) {
	if (recalculate)
		cb = ColourBook(img, k);
	switch (type) {
	case ORIG:
		imshow(title, cb.orig);
		break;
	case QUANT:
		imshow(title, cb.quantized);
		break;
	case PROD:
		imshow(title, cb.product);
		break;
	}
}

void Window::run() {
	namedWindow(title, WINDOW_AUTOSIZE);
	// Need a mouseCallBack so trackbar doesn't lag out
	setMouseCallback(title, onMouse, NULL);
	createButton("Show Original", onButtonOrig, this, QT_PUSH_BUTTON, 0);
	createButton("Show Quantized", onButtonQuant, this, QT_PUSH_BUTTON, 0);
	createButton("Show Product", onButtonProd, this, QT_PUSH_BUTTON, 0);
	createButton("Run", onButtonRun, this, QT_PUSH_BUTTON|QT_NEW_BUTTONBAR, 0);
	createTrackbar(trackbarName, title, &k, kmax, onTrackbar, this);
	draw(0);
	while (true) {
		int k = waitKey(10);
		if (k==27) break;
	}
}
