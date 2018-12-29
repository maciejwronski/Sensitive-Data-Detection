#include "stdafx.h"
#include "Censor.h"

// TODO: Gaussianblur throws exceptions on rotated image

cv::Mat Censor::SetRect(const std::vector<cv::Rect>& objbuffer, const cv::Mat& imgMatrix, const cv::Scalar& scalar)
{
	cv::Mat mat = imgMatrix;
	for (std::vector<cv::Rect>::const_iterator r = objbuffer.begin(); r != objbuffer.end(); r++)
		cv::rectangle(mat, cv::Rect(*r), scalar, 2, 2);
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
		
		cv::Size gaussSize;
		if ((*r).width % 2 == 0 && (*r).height % 2 == 0) {
			gaussSize.width = (*r).width - 1;
			gaussSize.height = (*r).height - 1;
		}
		else if ((*r).width % 2 == 0) {
			gaussSize.width = (*r).width - 1;
			gaussSize.height = (*r).height;
		}
		else if ((*r).height % 2 == 0) {
			gaussSize.width = (*r).width;
			gaussSize.height = (*r).height-1;
		}
		else  {
			gaussSize.width = (*r).width;
			gaussSize.height = (*r).height;
		}
		try {
			cv::GaussianBlur(mat, mat, gaussSize, 100);
		}
		catch(cv::Exception e){
			std::cout << "Error: " << e.what() << std::endl;
		}

	}
	return mat;
}
