#pragma once

#include "ObjectDetection.h"

class FaceDetection : public ObjectDetection  {
	std::string _eyeCascadeName = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_eye.xml";
	std::string _profileCascadeName = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_profileface.xml";
	bool LastIsEyeCascade = false;
	void DetectObjects() override;

	int CheckIfImageIsNotRotated() override;
	int CheckRotationByFindingDetail() override;
	int CheckRotationByRotatingImage() override;
public:
	FaceDetection(const std::string& filePath) : ObjectDetection(filePath) {
		_windowName = "Face Detection Window";
		_cascadeName = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
		additionalCascades.push_back(_profileCascadeName);
		additionalCascades.push_back(_eyeCascadeName);
		MethodToFindRotation = ByFindingDetail;
		maxWidth = 1000;
		maxHeight = 1000;
	};
};