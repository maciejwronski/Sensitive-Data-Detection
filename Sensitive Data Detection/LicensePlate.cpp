#pragma once
#include "stdafx.h"
#include "LicensePlate.h"

bool LicensePlateDetection::LoadImage(cv::Mat & matFile, const std::string & filePath)
{
	matFile = cv::imread(filePath);
	if (!matFile.data) {
		std::cout << Messages::CouldntLoadImage(filePath);
		return false;
	}
	else return true;
}

void LicensePlateDetection::CreateWindow(const std::string & windowName) const
{
	cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE*0.3);
}

cv::Mat LicensePlateDetection::ReturnGrayImage(cv::Mat & Image)
{
	cv::Mat grayImg;
	cv::cvtColor(Image, grayImg, cv::COLOR_BGR2GRAY);
		cv::imshow("Gray Image", grayImg);
	return grayImg;
}

cv::Mat LicensePlateDetection::ReturnImageWithoutContours(cv::Mat & Image)
{
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat newImage = ReturnGrayImage(Image);
	cv::Mat newImage2;
	cv::threshold(newImage, newImage2, 127, 255, 0);
	cv::findContours(newImage2, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	return newImage2;
}

float LicensePlateDetection::PercentageOfWhitePixels(cv::Mat& img, cv::RotatedRect& rotrect, cv::Rect& roi)
{
		int cnt = 0;
		std::vector<cv::Point2f> points(4);
		rotrect.points(points.data());
		float area = rotrect.size.width * rotrect.size.height;
		for (int i = roi.x; i < roi.x + roi.width; i++) {
			for (int j = roi.y; j < roi.y + roi.height; j++) {
				cv::Point2f point(i, j);
				cv::Scalar colour = img.at<uchar>(point);
				if (colour.val[0] == 255 && (cv::pointPolygonTest(points, cv::Point2f(i, j), false) >= 0.0)) {
					cnt++;
				}
			}
		}
		std::cout << cnt / area << std::endl;
		return cnt / area;
}

void LicensePlateDetection::ShowObjects(int censorType)
{
	if (!LoadImage(matFile, _filePath))
		return;
	CreateWindow(_windowName);
	cv::Mat FinalMat;
	matFile.copyTo(FinalMat);
	cv::imshow("Original Image", matFile);
	std::vector<std::vector<cv::Point>> possiblePlates = DetectPossiblePlates(matFile);
	std::vector<std::vector<cv::Point>> possiblePlatesByParentContours = ChildrenContours(FinalMat);
	std::cout << "Possible num of plates after first part: " <<  possiblePlates.size() << std::endl;
	std::cout << "Possible num of plates by finding numbers inside of license plate" << possiblePlatesByParentContours.size() << std::endl;
	int index;
	cv::Rect finalRect;
	if (possiblePlates.empty() && possiblePlatesByParentContours.empty())
		return;
	else if (possiblePlates.empty()) {
		index = 0;
		finalRect = cv::boundingRect(possiblePlatesByParentContours[index]);
	}
	else if (possiblePlatesByParentContours.empty()) {
		index = 0;
		finalRect = cv::boundingRect(possiblePlates[index]);
	}
	else {
		index = TryToFindIndex(possiblePlates, possiblePlatesByParentContours);
		finalRect = cv::boundingRect(possiblePlates[index]);
	}

	//std::cout << "THIS: " << cv::contourArea(possiblePlates[index]) << std::endl;
	ShowFinalImage(FinalMat, finalRect, censorType);

	//cv::waitKey(0);
}

void LicensePlateDetection::ShowFinalImage(cv::Mat& FinalImg, cv::Rect& finalRect, int censorType) {
	Censor censor((Censor::Types)censorType);
	std::vector<cv::Rect> objbuffer1;
	objbuffer1.push_back(finalRect);
	switch (censor.currType) {
	case censor.GaussianBlur:
		censor.SetGaussianBlur(objbuffer1, FinalImg);
		break;
	case censor.Rect:
		censor.SetRect(objbuffer1, FinalImg, cv::Scalar(0, 0, 255));
		break;
	case censor.FilledRect:
		censor.SetFilledRect(objbuffer1, FinalImg, cv::Scalar(0, 0, 255));
		break;

	}
#if !FOLDERS
    cv::rectangle(FinalImg, cv::Point(finalRect.x, finalRect.y), cv::Point(finalRect.x + finalRect.width, finalRect.y + finalRect.height), cv::Scalar(0, 255, 0), 1);
	cv::imshow("Final Image", FinalImg);
	cv::waitKey(0);
#else
	std::string name{ std::to_string(counter) + ".jpg" };
	std::string all = ResultPath + name;
	cv::imwrite(all, FinalImg);
	counter++;
#endif
}
int LicensePlateDetection::TryToFindIndex(const std::vector<std::vector<cv::Point>>& possiblePlates, const std::vector<std::vector<cv::Point>>& possiblePlateByContours) {
	int index = 0;
	double test = 0;
	double min = 9999;
	cv::Rect rect;
	for (int i = 0; i < possiblePlates.size(); i++) {
		for (int j = 0; j < possiblePlateByContours.size(); j++) {
			test = cv::matchShapes(possiblePlates[i], possiblePlateByContours[j], cv::CONTOURS_MATCH_I1, 0);
			if (test < min) {
				min = test;
				index = i;
			}
		}
	}
	return index;
}
std::vector<std::vector<cv::Point>> LicensePlateDetection::DetectPossiblePlates(cv::Mat& img) {
		cv::Mat gray;
		cv::Mat ret;
		cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
		cv::threshold(gray, ret, 127.0, 255.0, 0);
		cv::Mat mask(img.size(), CV_8UC1);
		std::vector<std::vector<cv::Point>> count;
		std::vector<std::vector<cv::Point>> possiblePlates;
		std::vector<cv::Vec4i> hierarchy;
		cv::findContours(ret, count, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
		cv::Mat temp = img;
		for(int i=0; i<count.size(); i++){
				cv::drawContours(temp, count, i, cv::Scalar(0, 255, 0), 2);
				cv::drawContours(mask, count, i, 255, -1);
		}
		//cv::imshow("Image with drawn contours", temp);
		//cv::imshow("Image with applied mask", mask);

		std::vector<cv::RotatedRect> minRect(count.size());
		for (size_t i = 0; i < count.size(); i++){
			minRect[i] = cv::minAreaRect(cv::Mat(count[i]));
			if (MinContourArea < cv::contourArea(count[i]) && cv::contourArea(count[i]) < MaxContourArea) {
				cv::Rect rect = cv::boundingRect(count[i]);
				if (ReturnNumOfWhitePixels(mask, rect, minRect[i]) < 0.4f || !HasMoreOrFourContours(count, i) || rect.width > rect.height*6 || rect.height*2 > rect.width || rect.height > rect.width ||
					PercentageOfWhitePixels(mask, minRect[i], rect) < 0.80f)
					continue;
				cv::rectangle(mask, cv::Point(rect.x, rect.y), cv::Point(rect.x + rect.width, rect.y + rect.height), cv::Scalar(0, 255, 0), 1);
				cv::Point2f rect_points[4];
				minRect[i].points(rect_points);
				for (int j = 0; j < 4; j++) {
					cv::line(mask, rect_points[j], rect_points[(j + 1) % 4], (0, 255, 0), 1, 8);
				}
				possiblePlates.push_back(count[i]);
			}
		}
		//cv::imshow("Image with applied mask and possible plates", mask);
		return possiblePlates;
}

std::vector<std::vector<cv::Point>> LicensePlateDetection::ChildrenContours(const cv::Mat& img) {
	cv::Mat ret;
	cv::Mat gray;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, ret, 127.0, 255.0, 0);
	cv::Mat mask(img.size(), CV_8UC1);
	std::vector<std::vector<cv::Point>> contours;
	std::vector<std::vector<cv::Point>> contoursOutput;
	std::vector<cv::Vec4i> hierarchy;
	std::map<int, int> counts;
	cv::findContours(ret, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++) {
		if (cv::contourArea(contours[i]) > 20 && hierarchy[i][3] != -1) {
			counts[hierarchy[i][3]]++;
		}
	}
	for (auto const& x : counts)
	{
		cv::Rect rect = cv::boundingRect(contours[x.first]);
		if (x.second > 3 && rect.width > rect.height) {
			contoursOutput.push_back(contours[x.first]);
		}
		
	}

	return contoursOutput;
}

bool LicensePlateDetection::HasMoreOrFourContours(std::vector<std::vector<cv::Point>>& rect, int index) {
	std::vector<std::vector<cv::Point>> test(1);
	cv::approxPolyDP(rect[index], test[0], 4, true);

	if (test[0].size() < 4)
		return false;
	return true;
}
double LicensePlateDetection::ReturnNumOfWhitePixels(cv::Mat& img, cv::Rect& rect, cv::RotatedRect& rotrect) {
	double countofWhitePixelsInRect = 0;
	double countofWhitePixelsInsideRotated = 0;
	double countofallpixels = 0;
	std::vector<cv::Point2f> points(4);
	rotrect.points(points.data());

	for (int i = rect.x; i < rect.x + rect.width; i++) {
		for (int j = rect.y; j < rect.y + rect.height; j++) {
			cv::Point2f point(i, j);
			cv::Scalar colour = img.at<uchar>(point);
			countofallpixels++;
			if (colour.val[0] == 255 && (cv::pointPolygonTest(points, cv::Point2f(i, j), false) >= 0.0)) {
				countofWhitePixelsInRect++;
				if (DoesRectangleContainPoint(rotrect, point)) {
					countofWhitePixelsInsideRotated++;
				}
			}
		}
	}

	if (countofWhitePixelsInsideRotated >= countofWhitePixelsInRect*0.5) {
		return float(countofWhitePixelsInRect / countofallpixels);
	}
	return 0;
}
bool LicensePlateDetection::DoesRectangleContainPoint(cv::RotatedRect& rectangle, cv::Point2f& point) {

	cv::Point2f cornersOfRect[4];
	rectangle.points(cornersOfRect);

	cv::Point2f* lastItemPointer = (cornersOfRect + sizeof cornersOfRect / sizeof cornersOfRect[0]);
	std::vector<cv::Point2f> contour(cornersOfRect, lastItemPointer);

	double indicator = cv::pointPolygonTest(contour, point, false);
	bool rectangleContainsPoint = (indicator >= 0);
	return rectangleContainsPoint;
}
