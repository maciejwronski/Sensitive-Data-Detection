#pragma once

#include <opencv2/opencv.hpp>
#include "messages.h"
#include "Censor.h"
#include <vector>

#define FOLDERS 1
#if FOLDERS
static int counter = 0;
#endif
#if FOLDERS
const std::string ResultPath{ "C:\\Users\\Maciej\\Documents\\Visual Studio 2017\\Projects\\Sensitive Data Detection\\Sensitive Data Detection\\Sensitive Data Detection\\cars_test_images\\result_eu\\" };
#endif

class LicensePlateDetection {
	cv::Mat matFile;
	std::string _filePath;
	std::string _windowName = "Detection Window";

	const int MaxContourArea = 5000;
	const int MinContourArea = 500;

	bool LoadImage(cv::Mat& matFile, const std::string& filePath);
	void CreateWindow(const std::string& windowName) const;
	cv::Mat ReturnGrayImage(cv::Mat& Image);
	cv::Mat ReturnImageWithoutContours(cv::Mat& Image);
	float PercentageOfWhitePixels(cv::Mat& img, cv::RotatedRect& rotrect,cv::Rect& rect);
public:
	LicensePlateDetection(const std::string& filePath) : _filePath(filePath) {};
	void ShowObjects(int  censorType);
	void ShowFinalImage(cv::Mat & FinalImg, cv::Rect & finalRect, int censorType);
	int TryToFindIndex(const std::vector<std::vector<cv::Point>>& possiblePlates, const std::vector<std::vector<cv::Point>>& possiblePlateByContours);
	std::vector<std::vector<cv::Point>> DetectPossiblePlates(cv::Mat& img);
	std::vector<std::vector<cv::Point>> ChildrenContours(const cv::Mat & img);
	bool HasMoreOrFourContours(std::vector<std::vector<cv::Point>>& rect, int index);
	double ReturnNumOfWhitePixels(cv::Mat & img, cv::Rect & rect, cv::RotatedRect& rotrect);
	bool DoesRectangleContainPoint(cv::RotatedRect& rectangle, cv::Point2f& point);

};