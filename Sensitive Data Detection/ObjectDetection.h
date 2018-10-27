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

	std::vector<std::string> cascadeVector;
	std::vector<cv::Rect> objbuffer1;
	std::vector<cv::Rect> objbuffer2;
	std::vector<cv::Rect> objbuffer3;

	std::string _windowName = "Detection Window";
	std::string _cascadeName = "C:\\opencv\\sources\\data\\haarcascades\\";
	std::string _filePath;

	cv::CascadeClassifier _cascade;
	cv::Mat matFile;

	int maxWidth = 100;
	int maxHeight = 100;
	int minWidth = 1;
	int minHeight = 1;


	void CreateWindow(const std::string& windowName) const;
	bool LoadCascade(cv::CascadeClassifier& cascade,const std::string& cascadeName) const;
	bool LoadImage(cv::Mat& matFile, const std::string& filePath);
	std::vector<cv::Rect> EliminateFalsePositives(std::vector<cv::Rect>& faceCascade, std::vector<cv::Rect>& eyeCascade);
	int CheckIfImageIsNotRotated();
	float Rad2Deg(float rad);
	cv::Mat RotateImage(cv::Mat& Image, float angle);
	cv::Mat ReturnImageWithMostPossibleObjects(cv::CascadeClassifier & cascade, const cv::Mat & matFile);
	virtual cv::Mat DetectObjects(cv::CascadeClassifier& cascade,cv::Mat& matFile, std::vector<cv::Rect>& objbuffer1, std::vector<cv::Rect>& objbuffer2, std::vector<cv::Rect>& objbuffer3, const int& minWidth, const int& minHeight, const int& maxWidth, const int& maxHeight) ;
public:
	ObjectDetection(const std::string& filePath) : _filePath(filePath) {};
	void ShowObjects();
};