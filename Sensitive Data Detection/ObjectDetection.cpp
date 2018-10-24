#include "stdafx.h"
#include "ObjectDetection.h"

bool ObjectDetection::LoadCascade(cv::CascadeClassifier& cascade,const std::string& cascadeName) const
{
	if (!(cascade.load(cascadeName))) {
		std::cout << Messages::CouldntLoadCascade(cascadeName);
		return false;
	}
	else return true;
}

bool ObjectDetection::LoadImage(cv::Mat& matFile, const std::string& filePath)
{
	matFile = cv::imread(filePath);
	if (!matFile.data) {
		std::cout << Messages::CouldntLoadImage(filePath);
		return false;
	}
	else return true;
}
void ObjectDetection::CreateWindow(const std::string& windowName) const {
	cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE*0.3);
}
int ObjectDetection::CheckRotationByRotatingImage(const cv::Mat& matFile) const
{
	return 0;
}
void ObjectDetection::DetectObjects(cv::CascadeClassifier& cascade, cv::Mat& matFile, std::vector<cv::Rect>& objbuffer, const int& minWidth, const int& minHeight, const int& maxWidth, const int& maxHeight)  {
	cv::Mat matGray;
	cv::cvtColor(matFile, matGray, cv::COLOR_BGR2GRAY);
	cascade.detectMultiScale(matGray, objbuffer, 1.1, 2, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));   
	if(!objbuffer.empty())
		std::cout << Messages::FoundByCascade(_cascadeName);
}
int ObjectDetection::CheckIfImageIsNotRotated()
{
	return 0;
}
float ObjectDetection::Rad2Deg(float rad)
{
	return rad * 180 / CV_PI;
}
cv::Mat ObjectDetection::RotateImage(cv::Mat &Image, float angle)
{
	cv::Mat src = Image;

	cv::Point2f center((src.cols - 1) / 2.0, (src.rows - 1) / 2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), src.size(), angle).boundingRect2f();
	rot.at<double>(0, 2) += bbox.width / 2.0 - src.cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - src.rows / 2.0;
	cv::Mat dst;
	cv::warpAffine(src, dst, rot, bbox.size());

	return dst;
}
void ObjectDetection::ShowObjects()
{
	if (!LoadImage(matFile, _filePath) || !LoadCascade(_cascade, _cascadeName))
		return;
	CreateWindow(_windowName);
	DetectObjects(_cascade, matFile, objbuffer, minWidth, minHeight, maxWidth, maxHeight);
	Censor censor;
	censor.SetFilledRect(objbuffer, matFile);
	cv::imshow(_windowName, matFile);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
