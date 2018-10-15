#pragma once

#include "ObjectDetection.h"

class FaceDetection : public ObjectDetection  {

public:
	FaceDetection(const std::string& filePath) : ObjectDetection(filePath) {
		_windowName = "Face Detection Window";
		_cascadeName = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
		maxWidth = 100;
		maxHeight = 100;
	};
};