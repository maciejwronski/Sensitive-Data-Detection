#include "stdafx.h"
#include "ObjectDetection.h"

bool ObjectDetection::LoadCascade(cv::CascadeClassifier& cascade, const std::string& cascadeName) const
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
void ObjectDetection::EliminateFalsePositives(std::vector<cv::Rect>& faceVector, std::vector<cv::Rect>& eyeVector)
{
	int val = 0;
	std::vector<int> indexes;
	for (int i = 0; i < eyeVector.size(); i++) {
		val = 0;
		for (int j = 0; j < faceVector.size(); j++) {
			if ((eyeVector[i] | faceVector[j]) == faceVector[j]) {
				val++;
				break;
			}
		}
		if (val == 0) {
			indexes.push_back(i);
		}
	}
	std::sort(indexes.begin(), indexes.end());
	for (int i = indexes.size()-1; i >= 0; i--) {
		eyeVector[indexes[i]] = eyeVector.back();
		eyeVector.pop_back();
	}
}

void ObjectDetection::CreateWindow(const std::string& windowName) const {
	cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE*0.3);
}

cv::Mat ObjectDetection::ReturnImageWithMostPossibleObjects(cv::CascadeClassifier& cascade, const cv::Mat& matFile, float angle) {
	int Max = 0;
	int index = 0;
	std::vector <cv::Rect> objbuffer;
	for (int i = 0; i < 360; i += angle) {
		std::cout << "SEARCHING FOR " << i << std::endl;
		cv::Mat img = matFile;
		img = RotateImage(img, i);
		cascade.detectMultiScale(img, objbuffer, 1.1, 2, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
		if (!objbuffer.empty()) {
			if (objbuffer.size() > Max) {
				Max = objbuffer.size();
				index = i;
			}
		}
		objbuffer.clear();
	}
	cv::Mat img = matFile;
	cv::Mat img2;
	img2 = RotateImage(img, index);
	img2 = CropWhiteBorder(img2);
	return img2;
}

cv::Point2f ObjectDetection::FindRotatedPointByDegrees(const cv::Point2f & point, const cv::Point2f & AxisPoint, int degree)
{
	cv::Point2f pointnew;
	pointnew.x = cos(Deg2Rad(degree)) * (point.x - AxisPoint.x) - sin(Deg2Rad(degree)) * (point.y - AxisPoint.y) + AxisPoint.x;
	pointnew.y = sin(Deg2Rad(degree)) * (point.x - AxisPoint.x) + cos(Deg2Rad(degree)) * (point.y - AxisPoint.y) + AxisPoint.y;
	return pointnew;
}

cv::Mat ObjectDetection::DetectObjects(cv::CascadeClassifier& cascade, cv::Mat& matFile, std::vector<cv::Rect>& objbuffer1, std::vector<cv::Rect>& objbuffer2, std::vector<cv::Rect>& objbuffer3, const int& minWidth, const int& minHeight, const int& maxWidth, const int& maxHeight)  {
	cv::Mat matGray;
	cv::cvtColor(matFile, matGray, cv::COLOR_BGR2GRAY);
	cascade.detectMultiScale(matGray, objbuffer1, 1.1, 2, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));   
	if (!objbuffer1.empty()) {
		std::cout << Messages::FoundByCascade(_cascadeName);
		return matFile;
	}
}
int ObjectDetection::CheckIfImageIsNotRotated()
{
	return 0;
}
float ObjectDetection::Rad2Deg(const float& rad)
{
	return rad * 180 / CV_PI;
}

float ObjectDetection::Deg2Rad(const float& deg)
{
	return deg * CV_PI / 180;
}

cv::Mat ObjectDetection::RotateImage(cv::Mat &Image, double angle)
{
	cv::Mat src = Image;

	cv::Point2f center((src.cols - 1) / 2.0, (src.rows - 1) / 2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), src.size(), angle).boundingRect2f();
	rot.at<double>(0, 2) += bbox.width / 2.0 - src.cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - src.rows / 2.0;
	cv::Mat dst;
	cv::warpAffine(src, dst, rot, bbox.size(), cv::INTER_LINEAR, cv::BORDER_TRANSPARENT, cv::Scalar(255,255,255) );
	dst = CropWhiteBorder(dst);
	return dst;
}

cv::Mat ObjectDetection::RotateImage(cv::Mat &Image, float angle, const cv::Point2f& point)
{
	cv::Mat src = Image;
	

	cv::Mat rot = cv::getRotationMatrix2D(point, angle, 1.0);
	cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), src.size(), angle).boundingRect2f();
	rot.at<double>(0, 2) += bbox.width / 2.0 - src.cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - src.rows / 2.0;
	cv::Mat dst;
	cv::warpAffine(src, dst, rot, bbox.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
	dst = CropWhiteBorder(dst);
	return dst;
}

cv::Mat ObjectDetection::CropWhiteBorder(cv::Mat& Image) {
	std::vector<cv::Point> points;
	points.reserve(Image.rows*Image.cols);
	for (int j = 0; j < Image.rows; ++j)
		for (int i = 0; i < Image.cols; ++i)
		{
			if (Image.at<cv::Vec3b>(j, i) != cv::Vec3b(255, 255, 255))
			{
				points.push_back(cv::Point(i, j));
			}
		}
	cv::Rect bb = cv::boundingRect(points);
	return Image(bb);
}
cv::Point2f ObjectDetection::CalculateMiddleOfTwoPoints(const cv::Point2f& point1,const cv::Point2f& point2) {
	return cv::Point2f((point1.x + point2.x) / 2.0, (point1.y + point2.y) / 2);
}

void ObjectDetection::ShowObjects(int  censorType)
{
	if (!LoadImage(matFile, _filePath) || !LoadCascade(_cascade, _cascadeName))
		return;
	CreateWindow(_windowName);
	//cv::imshow("Original Image", matFile);
	matFile = CropWhiteBorder(matFile);

	CropWhiteBorder(matFile);
	matFile = DetectObjects(_cascade, matFile, objbuffer1, objbuffer2, objbuffer3, minWidth, minHeight, maxWidth, maxHeight);
	EliminateFalsePositives(objbuffer1, objbuffer3);
	Censor censor((Censor::Types)censorType);
	std::cout << (Censor::Types)censorType << std::endl;
	switch (censor.currType) {
		case censor.GaussianBlur:
			censor.SetGaussianBlur(objbuffer1, matFile);
		break;
		case censor.Rect:
			censor.SetRect(objbuffer1, matFile, cv::Scalar(255, 0, 0));
			censor.SetRect(objbuffer2, matFile, cv::Scalar(0, 255, 0));
			censor.SetRect(objbuffer3, matFile, cv::Scalar(0, 0, 255));
		break;
		case censor.FilledRect:
			censor.SetFilledRect(objbuffer1, matFile, cv::Scalar(255, 0, 0));
			censor.SetFilledRect(objbuffer2, matFile, cv::Scalar(0, 255, 0));
			censor.SetFilledRect(objbuffer3, matFile, cv::Scalar(0, 0, 255));
		break;

	}
	matFile = CropWhiteBorder(matFile);
	//cv::imshow(_windowName, matFile);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
