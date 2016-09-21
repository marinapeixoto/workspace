#pragma once
#include "inc.h"
#include "FrameProcessor.h"

class VideoProcessor {
private:
	cv::VideoCapture capture;
	// callback function
	void (*process)(cv::Mat&, cv::Mat&);
	bool callIt; // 回调函数是否被调用
	std::string windowNameInput;
	std::string windowNameOutput;
	int delay;
	long fnumber;
	long frameToStop;
	bool stop;
	FrameProcessor* frameProcessor;
	std::vector<std::string> images;
	std::vector<std::string>::const_iterator itImg;
public:
	VideoProcessor() :callIt(true), delay(0), fnumber(0), stop(false), frameToStop(-1) {}
	// 设置回调函数
	void setFrameProcessor(void(*frameProcesingCallback)(cv::Mat&, cv::Mat&)) {
		frameProcessor = 0;
		process = frameProcesingCallback;
		callProcess();
	}
	void setFrameProcessor(FrameProcessor* frameProcessorPtr) {
		process = 0;
		frameProcessor = frameProcessorPtr;
		callProcess();
	}
	bool setInput(std::string filename) {
		fnumber = 0;
		capture.release();
		images.clear();
		return capture.open(filename);
	}
	bool setInput(const std::vector<std::string>& imgs) {
		fnumber = 0;
		capture.release();
		images = imgs;
		return true;
	}
	void displayInput(std::string wn) {
		windowNameInput = wn;
		cv::namedWindow(windowNameInput);
	}
	void displayOutput(std::string wn) {
		windowNameOutput = wn;
		cv::namedWindow(windowNameOutput);
	}
	void dontDisplay() {
		cv::destroyWindow(windowNameInput);
		cv::destroyWindow(windowNameOutput);
		windowNameInput.clear();
		windowNameOutput.clear();
	}


	void run() {
		cv::Mat frame;
		cv::Mat output;
		if (!isOpened()) {
			return;
		}
		stop = false;
		while (!isStopped()) {
			if (!readNextFrame(frame)) {
				break;
			}
			if (windowNameInput.length()!=0) {
				cv::imshow(windowNameInput, frame);
			}
			if (callIt) {
				if (process) {
					process(frame, output);
				} else if (frameProcessor) {
					frameProcessor->process(frame, output);
				}

				fnumber++;
			} else {
				output = frame;
			}
			if (windowNameOutput.length()!=0) {
				cv::imshow(windowNameOutput, output);
			}
			if (delay>=0&&cv::waitKey(delay)>=0) {
				stopIt();
			}
			if (frameToStop>=0 && getFrameNumber()== frameToStop) {
				stopIt();
			}
		}
	}
	void stopIt() {
		stop = true;
	}
	bool isStopped() {
		return stop;
	}
	bool isOpened() {
		return capture.isOpened() || !images.empty();
	}
	void setDelay(int d) {
		delay = d;
	}
	void callProcess() {
		callIt = true;
	}
	void dontCallProcess() {
		callIt = false;
	}
	void StopAtFrameNo(long frame) {
		frameToStop = frame;
	}
	long getFrameNumber() {
		long fnumber = static_cast<long>(capture.get(CV_CAP_PROP_POS_FRAMES));
		return fnumber;
	}
private:
	bool readNextFrame(cv::Mat& frame) {
		if (images.size()==0) {
			return capture.read(frame);
		} else {
			if (itImg!=images.end()) {
				frame = cv::imread(*itImg);
				itImg++;
				return frame.data != 0;
			} else {
				return false;
			}
		}
	}
};