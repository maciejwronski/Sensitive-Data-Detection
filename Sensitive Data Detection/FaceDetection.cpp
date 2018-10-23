#include "stdafx.h"
#include "FaceDetection.h"

void FaceDetection::DetectObjects() {
	cv::Mat matGray;
	cv::cvtColor(matFile, matGray, cv::COLOR_BGR2GRAY);
	_cascade.detectMultiScale(matGray, objbuffer, 1.1, 2, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));

	if (!objbuffer.empty())
		return;
	std::cout << Messages::UnableToFindWithCascade(_cascadeName);
	if (additionalCascades.empty())
		return;

	for (auto& cascade: additionalCascades) {
		std::cout << Messages::UsingNewCascade(cascade);
		_cascadeName = cascade;
		if (!LoadCascade())
			return;
		_cascade.detectMultiScale(matGray, objbuffer, 1.1, 2, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
		if (!objbuffer.empty()) {
			if (_cascadeName == _eyeCascadeName)
				LastIsEyeCascade;
			break;
		}
		else
			std::cout << Messages::UnableToFindWithCascade(_cascadeName);
	}
	CreateWindow();
}

int FaceDetection::CheckIfImageIsNotRotated() {
	switch (MethodToFindRotation) {
	case ByFindingDetail:
		return CheckRotationByFindingDetail();
		break;
	case ByRotatingImage:
		return CheckRotationByRotatingImage();
		break;
	}
	return 0;
}

int FaceDetection::CheckRotationByFindingDetail() {
	return 0;
}
int FaceDetection::CheckRotationByRotatingImage() {
	return 0;
}