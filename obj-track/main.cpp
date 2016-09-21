#include "prj_inc.h"

void canny(cv::Mat& img, cv::Mat& out) {
	if (img.channels()==3) {
		cv::cvtColor(img, out, CV_BGR2GRAY);
	}
	cv::Canny(out, out, 100, 200);
	cv::threshold(out, out, 128, 255, cv::THRESH_BINARY_INV);
}

int main() {
	VideoProcessor processor;
	FeatureTracker tracker;
	processor.setInput("./bike.avi");
	processor.displayInput("Current Frame"); 
	processor.displayOutput("Output Frame");

	processor.setDelay(100);
	processor.setFrameProcessor(&tracker);
	processor.run();
}