#include "stdafx.h"
#include "FaceDetection.h"

cv::Mat FaceDetection::DetectObjects(cv::CascadeClassifier& cascade, cv::Mat& matFile, std::vector<cv::Rect>& objbuffer1, std::vector<cv::Rect>& objbuffer2, std::vector<cv::Rect>& objbuffer3, const int& minWidth, const int& minHeight, const int& maxWidth, const int& maxHeight) {
	if (MethodToFindRotation == ByRotatingImage) {
		for (const std::string& newCascade : cascadeVector) {
			std::cout << Messages::UsingNewCascade(newCascade);
			if (!LoadCascade(cascade, newCascade)) {
				return matFile;
			}
			cv::Mat tempImage = matFile;
			cv::Mat tempImage2;
			//cv::cvtColor(tempImage, matGray, cv::COLOR_BGR2GRAY);
			tempImage2 = ReturnImageWithMostPossibleObjects(cascade, tempImage);
			if(newCascade == _mainCascade)
				cascade.detectMultiScale(tempImage2, objbuffer1, 1.1, 2, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
			else if (newCascade == _profileCascadeName)
				cascade.detectMultiScale(tempImage2, objbuffer2, 1.1, 2, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
			if (newCascade == _eyeCascadeName)
				cascade.detectMultiScale(tempImage2, objbuffer3, 1.1, 2, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));

			if (newCascade == _mainCascade && objbuffer1.empty()
				|| newCascade == _profileCascadeName && objbuffer2.empty() || newCascade == _eyeCascadeName && objbuffer3.empty()) {
				std::cout << Messages::UnableToFindWithCascade(newCascade);
			}
			else {
				std::cout << Messages::FoundWithRotator(newCascade);
				if (newCascade == _mainCascade && !objbuffer1.empty()) {
						std::cout << "Applying next cascades on main cascade" << std::endl;
						cv::Mat tempImage = tempImage2;
						for (int i = 1; i < 3; i++) {
							if (!LoadCascade(cascade, cascadeVector[i])) 
								break;
							if (i == 1) {
								cascade.detectMultiScale(tempImage2, objbuffer2, 1.1, 2, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
								if (objbuffer2.empty())
									std::cout << Messages::UnableToFindWithCascade(cascadeVector[i]);
								else {
									std::cout << Messages::FoundWithRotatorOnAppliedCascade(cascadeVector[i]);
								}
							}
							else if (i == 2) {
								cascade.detectMultiScale(tempImage2, objbuffer3, 1.1, 2, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
								if (objbuffer3.empty())
									std::cout << Messages::UnableToFindWithCascade(cascadeVector[i]);
								else {
									std::cout << Messages::FoundWithRotatorOnAppliedCascade(cascadeVector[i]);
								}
							}
					}
					return tempImage2;
				}
			}
		}
		objbuffer1.insert(std::end(objbuffer1), std::begin(objbuffer2), std::end(objbuffer2));
		objbuffer1.insert(std::end(objbuffer1), std::begin(objbuffer3), std::end(objbuffer3));
	}
	else if (MethodToFindRotation == ByFindingDetail) {
		cv::Mat tempImage = matFile;
		for (const std::string& newCascade : cascadeVector) {
			if (!LoadCascade(cascade, newCascade))
				return matFile;
			if (newCascade == _eyeCascadeName) {
				cascade.detectMultiScale(tempImage, objbuffer1, 1.1, 4, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
			}
			else if (newCascade == _profileCascadeName) {
				cascade.detectMultiScale(tempImage, objbuffer1, 1.1, 1, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
			}
			if (!objbuffer1.empty()) {
				std::cout << Messages::FoundByCascade(newCascade);
				if (newCascade == _eyeCascadeName) {
					// what if image is rotated by 180?
					if (objbuffer1.size() > 2)
						FindTwoClosestRectangles(objbuffer1);
					float Degrees = CheckRotationByFindingDetail(tempImage, objbuffer1);
					Censor censor;
					censor.SetFilledRect(objbuffer1, tempImage, cv::Scalar(0, 0, 0));
					cv::imshow("Rotated image with Details", tempImage);
					std::cout << "DEGREES" << Degrees << std::endl;
					tempImage = RotateImage(matFile, -Degrees); ///// function works only on clockwise

					if (!LoadCascade(cascade, _mainCascade))
						return matFile;
					cascade.detectMultiScale(tempImage, objbuffer1, 1.1, 4, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
					return tempImage;
				}
			}
			else {
				std::cout << Messages::UnableToFindWithCascade(newCascade);
			}
		}
	}
}


int FaceDetection::CheckRotationByFindingDetail(const cv::Mat& originalImage, std::vector<cv::Rect>& objects) {
	if (objects.size() == 2) { // found two eyes, calculate distance between br, then cos and angle.
		cv::Mat img;
		img = originalImage;
		SetWidthAndHeightSame(objects[0], objects[1]);
		std::vector<cv::Point> points(2);
		
		points[0] = objects[0].br();
		points[1] = objects[1].br();
		cv::line(img, points[0], points[1], cv::Scalar(255, 0, 0));

		float len = sqrt(pow(points[0].x - points[1].x, 2) + pow(points[0].y - points[1].y, 2));
		float a = points[0].x - points[1].x;
		
		return Rad2Deg(cos(a / len));
	}
	else return 0;

} 
void FaceDetection::SetWidthAndHeightSame(cv::Rect& obj, cv::Rect& obj2) {
	if (obj.width*obj.height < obj2.width*obj2.height) {
		obj2.width = obj.width;
		obj2.height = obj.height;
		//obj2.y = obj.y;
	}
	else {
		obj.width = obj2.width;
		obj.height = obj2.height;
		//obj.y = obj2.y;
	}
}

void FaceDetection::FindTwoClosestRectangles(std::vector<cv::Rect>& vec) {
	int indexes[2];
	int currMinDistance = 9999;
	int tempDist = 0;
	std::vector<cv::Point> vecbr;
	std::vector<cv::Point> vectl;
	for (int i = 0; i < vec.size(); i++) {
		vecbr.push_back(vec[i].br());
		vectl.push_back(vec[i].tl());
	}

	for (int i = 0; i < vec.size(); i++) {
		for (int j = 0; j != i, j < vec.size(); j++) {
			tempDist = sqrt(pow(vecbr[i].x - vectl[j].x, 2) + pow(vecbr[i].y - vectl[j].y, 2));
			if (tempDist < currMinDistance) {
				currMinDistance = tempDist;
				indexes[0] = i;
				indexes[1] = j;
			}
		}
	}
	for (int i = 0; i < vec.size(); i++) {
		if (i == indexes[0] || i == indexes[1])
			break;
		else {
			vec.erase(vec.begin() + i);
		}
	}
}