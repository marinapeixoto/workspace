#pragma once
#include "inc.h"

class FrameProcessor {
public:
	virtual void process(cv::Mat& input,cv::Mat& output)=0;
};