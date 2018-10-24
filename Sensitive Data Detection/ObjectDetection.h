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
		ByFindingDetail,
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
	bool LoadCascade(cv::CascadeClassifier& cascade,const std::string& cascadeName) const;
	bool LoadImage(cv::Mat& matFile, const std::string& filePath);
	void CreateWindow(const std::string& windowName) const;
	int CheckRotationByRotatingImage(const cv::Mat& matFile) const;
	int CheckIfImageIsNotRotated();
	float Rad2Deg(float rad);
	cv::Mat RotateImage(cv::Mat& Image, float angle);
	virtual void DetectObjects(cv::CascadeClassifier& cascade,cv::Mat& matFile, std::vector<cv::Rect>& objbuffer, const int& minWidth, const int& minHeight, const int& maxWidth, const int& maxHeight) ;
public:
	ObjectDetection(const std::string& filePath) : _filePath(filePath) {};
	void ShowObjects();
};