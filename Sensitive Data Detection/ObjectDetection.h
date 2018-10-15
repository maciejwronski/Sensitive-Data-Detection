#pragma once

#include <string>
#include <opencv2/opencv.hpp>
#include "messages.h"
#include <vector>
#include "Censor.h"

class ObjectDetection {
protected:
	std::string _windowName = "Detection Window";
	std::string _cascadeName = "C:\\opencv\\sources\\data\\haarcascades\\";
	std::string _filePath;
	cv::CascadeClassifier _cascade;
	cv::Mat matFile;
	std::vector<cv::Rect> objbuffer;


	int maxWidth = 1;
	int maxHeight = 1;
	bool LoadCascade();
	bool LoadImage();
	void CreateWindow();
	void DetectObjects();
public:
	ObjectDetection(const std::string& filePath) : _filePath(filePath) {};
	void ShowObjects();
};