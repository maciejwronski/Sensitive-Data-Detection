#pragma once

#include <string>
#include <opencv2/opencv.hpp>
#include "messages.h"
#include <vector>
#include "menu.h"
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


	bool LoadCascade(cv::CascadeClassifier& cascade, const std::string& cascadeName) const;
	bool LoadImage(cv::Mat& matFile, const std::string& filePath);
	cv::Mat RotateImage(cv::Mat& Image, double angle);
	cv::Mat RotateImage(cv::Mat & Image, float angle,const cv::Point2f& point);
	cv::Mat ReturnImageWithMostPossibleObjects(cv::CascadeClassifier & cascade, const cv::Mat & matFile, float angle);
	cv::Point2f FindRotatedPointByDegrees(const cv::Point2f& point, const cv::Point2f& AxisPoint, int degree);
	cv::Mat CropWhiteBorder(cv::Mat & Image);

	int CheckIfImageIsNotRotated();
	float Rad2Deg(const float& rad);
	float Deg2Rad(const float & deg);
	void EliminateFalsePositives(std::vector<cv::Rect>& faceCascade, std::vector<cv::Rect>& eyeCascade);
	void CreateWindow(const std::string& windowName) const;
	virtual cv::Mat DetectObjects(cv::CascadeClassifier& cascade,cv::Mat& matFile, std::vector<cv::Rect>& objbuffer1, std::vector<cv::Rect>& objbuffer2, std::vector<cv::Rect>& objbuffer3, const int& minWidth, const int& minHeight, const int& maxWidth, const int& maxHeight) ;
public:
	ObjectDetection(const std::string& filePath) : _filePath(filePath) {};
	cv::Point2f CalculateMiddleOfTwoPoints(const cv::Point2f & point1,const cv::Point2f & point2);
	void ShowObjects(int  censorType);
};
