#pragma once

#include <opencv2/opencv.hpp>

class Censor {

public:

	cv::Mat setRect(const std::vector<cv::Rect>& objbuffer, const cv::Mat& imgMatrix);
	cv::Mat setGaussianBlur(const std::vector<cv::Rect>& objbuffer, const cv::Mat& imgMatrix);
};