#include "stdafx.h"
#include "ObjectDetection.h"

bool ObjectDetection::LoadCascade()
{
	if (!(_cascade.load(_cascadeName))) {
		std::cout << Messages::CouldntLoadCascade(_cascadeName);
		return false;
	}
	else return true;
}
bool ObjectDetection::LoadImage()
{
	matFile = cv::imread(_filePath);
	if (!matFile.data) {
		std::cout << Messages::CouldntLoadImage(_filePath);
		return false;
	}
	else return true;
}
void ObjectDetection::CreateWindow() {
	cv::namedWindow(_windowName, cv::WINDOW_AUTOSIZE*0.3);
}
void ObjectDetection::DetectObjects() {
	cv::Mat matGray;
	cv::cvtColor(matFile, matGray, cv::COLOR_BGR2GRAY);
	_cascade.detectMultiScale(matGray, objbuffer, 1.1, 2, 0, cv::Size(maxWidth, maxHeight));
	cv::imshow(_windowName, matFile);                       
}
void ObjectDetection::ShowObjects()
{
	if (!LoadImage() || !LoadCascade())
		return;
	CreateWindow();
	DetectObjects();

	Censor censor;
	censor.setGaussianBlur(objbuffer, matFile);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
