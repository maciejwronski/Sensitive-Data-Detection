#pragma once

#include "ObjectDetection.h"

class FaceDetection : public ObjectDetection  {
	std::string _mainCascade = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	std::string _eyeCascadeName = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_eye.xml";
	std::string _profileCascadeName = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_profileface.xml";

	bool LastIsEyeCascade = false;
	void DetectObjects(cv::CascadeClassifier& cascade, cv::Mat& matFile, std::vector<cv::Rect>& objbuffer, const int& minWidth, const int& minHeight, const int& maxWidth, const int& maxHeight) override;

	bool TryToApplyFaceDetectorsAgain(cv::CascadeClassifier & cascade, cv::Mat & Image, const std::string & cascadeName);

	int CheckRotationByFindingDetail(const cv::Mat& originalImage, std::vector<cv::Rect>& objects);
	void SetWidthAndHeightSame(cv::Rect& obj, cv::Rect& obj2);
public:
	FaceDetection(const std::string& filePath) : ObjectDetection(filePath) {
		_windowName = "Final - Face Detection Window";
		_cascadeName = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
		additionalCascades.push_back(_profileCascadeName);
		additionalCascades.push_back(_eyeCascadeName);
		MethodToFindRotation = ByRotatingImage;
		maxWidth = 1000;
		maxHeight = 1000;
	};
};