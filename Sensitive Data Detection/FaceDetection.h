#pragma once

#include "ObjectDetection.h"

class FaceDetection : public ObjectDetection  {
	std::string _mainCascade = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	std::string _eyeCascadeName = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_eye.xml";
	std::string _profileCascadeName = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_profileface.xml";

	cv::Mat DetectObjects(cv::CascadeClassifier& cascade, cv::Mat& matFile, std::vector<cv::Rect>& objbuffer1, std::vector<cv::Rect>& objbuffer2, std::vector<cv::Rect>& objbuffer3, const int& minWidth, const int& minHeight, const int& maxWidth, const int& maxHeight) override;

	int CheckRotationByFindingDetail(const cv::Mat& originalImage, std::vector<cv::Rect>& objects, cv::Point2f& MiddleOfTwoPoints);
	void SetWidthAndHeightSame(cv::Rect& obj, cv::Rect& obj2);
	void FindTwoClosestRectangles(std::vector<cv::Rect>& vec);
	int FindRemainingAngle(const cv::Point2f & point1,const  cv::Point2f & point2,const cv::Point & axisPoint);
public:
	void ShowObjects(int  censorType);
	FaceDetection(const std::string& filePath) : ObjectDetection(filePath) {
		_windowName = "Final - Face Detection Window";
		_cascadeName = _mainCascade;

		cascadeVector.push_back(_mainCascade);
		cascadeVector.push_back(_profileCascadeName);
		cascadeVector.push_back(_eyeCascadeName);

		MethodToFindRotation = ByFindingDetail;

		maxWidth = 1000;
		maxHeight = 1000;
	};
};