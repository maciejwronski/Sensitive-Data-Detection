#include "stdafx.h"
#include "Censor.h"

cv::Mat Censor::SetRect(const std::vector<cv::Rect>& objbuffer, const cv::Mat& imgMatrix)
{
	cv::Mat mat = imgMatrix;
	for (std::vector<cv::Rect>::const_iterator r = objbuffer.begin(); r != objbuffer.end(); r++)
		cv::rectangle(mat, cv::Rect(*r), cv::Scalar(0, 0, 0), 2, 2, 0);
	return mat;
}
cv::Mat Censor::SetFilledRect(const std::vector<cv::Rect>& objbuffer, const cv::Mat& imgMatrix, const cv::Scalar& scalar) {
	cv::Mat mat = imgMatrix;
	for (std::vector<cv::Rect>::const_iterator r = objbuffer.begin(); r != objbuffer.end(); r++)
		cv::rectangle(mat, cv::Rect(*r), scalar, cv::FILLED);
	return mat;
}
cv::Mat Censor::SetGaussianBlur(const std::vector<cv::Rect>& objbuffer, const cv::Mat& imgMatrix)
{
	cv::Mat mat = imgMatrix;
	for (std::vector<cv::Rect>::const_iterator r = objbuffer.begin(); r != objbuffer.end(); r++){
		mat = imgMatrix(*r);
		try {
			cv::GaussianBlur(mat, mat, cv::Size((*r).width, (*r).height), 1000);
		}
		catch(cv::Exception e){

		}

	}
	return imgMatrix;
}
