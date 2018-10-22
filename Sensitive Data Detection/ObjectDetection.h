#pragma once

#include <string>
#include <opencv2/opencv.hpp>
#include "messages.h"
#include <vector>
#include "Censor.h"

class ObjectDetection {
protected:

	enum FindRotationOfGivenImage {
		ByRotatingImage,
		ByFindingDetail;
	};

	FindRotationOfGivenImage MethodToFindRotation;

	std::vector<std::string> additionalCascades;
	std::string _windowName = "Detection Window";
	std::string _cascadeName = "C:\\opencv\\sources\\data\\haarcascades\\";
	std::string _filePath;
	cv::CascadeClassifier _cascade;
	cv::Mat matFile;
	std::vector<cv::Rect> objbuffer;


	int maxWidth = 100;
	int maxHeight = 100;
	int minWidth = 1;
	int minHeight = 1;
	bool LoadCascade();
	bool LoadImage();
	void CreateWindow();
	virtual void DetectObjects();
	virtual int CheckIfImageIsNotRotated() = 0;
	virtual int CheckRotationByFindingDetail() = 0;
	virtual int CheckRotationByRotatingImage();
public:
	ObjectDetection(const std::string& filePath) : _filePath(filePath) {};
	void ShowObjects();
};