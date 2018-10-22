#pragma once

#include <opencv2/opencv.hpp>

class Censor {

public:

	cv::Mat SetRect(const std::vector<cv::Rect>& objbuffer, const cv::Mat& imgMatrix);
	cv::Mat SetFilledRect(const std::vector<cv::Rect>& objbuffer, const cv::Mat & imgMatrix);
	cv::Mat SetGaussianBlur(const std::vector<cv::Rect>& objbuffer, const cv::Mat& imgMatrix);
};