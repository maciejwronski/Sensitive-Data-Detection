#include "stdafx.h"
#include "Censor.h"

cv::Mat Censor::setRect(const std::vector<cv::Rect>& objbuffer, const cv::Mat& imgMatrix)
{
	cv::Mat mat = imgMatrix;
	for (std::vector<cv::Rect>::const_iterator r = objbuffer.begin(); r != objbuffer.end(); r++)
		cv::rectangle(mat, cv::Rect(*r), cv::Scalar(120, 5, 86), 2, 2, 0);
	return mat;
}
cv::Mat Censor::setGaussianBlur(const std::vector<cv::Rect>& objbuffer, const cv::Mat& imgMatrix)
{
	cv::Mat mat = imgMatrix;
	for (std::vector<cv::Rect>::const_iterator r = objbuffer.begin(); r != objbuffer.end(); r++){
		mat = imgMatrix(*r);
		cv::GaussianBlur(mat, mat, cv::Size((*r).width, (*r).height), 1000);
	}
	return imgMatrix;
}
