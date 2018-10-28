#pragma once

#include <opencv2/opencv.hpp>

class Censor {
public:
	enum Types {
		GaussianBlur,
		Rect,
		FilledRect
	};
	Types currType = FilledRect;
	Censor(Types value) : currType(value) {};
	cv::Mat SetRect(const std::vector<cv::Rect>& objbuffer, const cv::Mat& imgMatrix, const cv::Scalar& scalar);
	cv::Mat SetFilledRect(const std::vector<cv::Rect>& objbuffer, const cv::Mat & imgMatrix, const cv::Scalar& scalar);
	cv::Mat SetGaussianBlur(const std::vector<cv::Rect>& objbuffer, const cv::Mat& imgMatrix);
};