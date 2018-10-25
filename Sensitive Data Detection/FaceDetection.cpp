#include "stdafx.h"
#include "FaceDetection.h"

cv::Mat FaceDetection::DetectObjects(cv::CascadeClassifier& cascade, cv::Mat& matFile, std::vector<cv::Rect>& objbuffer, const int& minWidth, const int& minHeight, const int& maxWidth, const int& maxHeight) {
	if (MethodToFindRotation == ByRotatingImage) {
		for (const std::string& newCascade : cascadeVector) {
			std::cout << Messages::UsingNewCascade(newCascade);
			if (!LoadCascade(cascade, newCascade) || newCascade == _eyeCascadeName) // apply all cascades, but not detail one
				return matFile;
			cv::Mat tempImage = matFile;
			cv::Mat tempImage2;
			//cv::cvtColor(tempImage, matGray, cv::COLOR_BGR2GRAY);
			tempImage2 = ReturnImageWithMostPossibleObjects(cascade, tempImage);
			cascade.detectMultiScale(tempImage2, objbuffer, 1.1, 2, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));

			if (objbuffer.empty()) {
				std::cout << Messages::UnableToFindWithCascade(newCascade);
			}
			else {
				std::cout << Messages::FoundWithRotator(newCascade);
				return tempImage2;
			}
		}
	}
	else if (MethodToFindRotation == ByFindingDetail) {
		cv::Mat tempImage = matFile;
		for (const std::string& newCascade : cascadeVector) {
			if (!LoadCascade(cascade, newCascade))
				return matFile;
			if (newCascade == _eyeCascadeName) {
				cascade.detectMultiScale(tempImage, objbuffer, 1.1, 4, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
			}
			else if (newCascade == _profileCascadeName) {
				cascade.detectMultiScale(tempImage, objbuffer, 1.1, 1, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
			}
			if (!objbuffer.empty()) {
				std::cout << Messages::FoundByCascade(newCascade);
				if (newCascade == _eyeCascadeName) {
					// what if image is rotated by 180?
					float Degrees = CheckRotationByFindingDetail(tempImage, objbuffer);
					tempImage = RotateImage(matFile, -Degrees); ///// function works only on clockwise
					cv::imshow("Rotated image with Details", tempImage);
					if (!LoadCascade(cascade, _mainCascade))
						return matFile;
					cascade.detectMultiScale(tempImage, objbuffer, 1.1, 4, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
					return tempImage;
				}
			}
			else {
				std::cout << Messages::UnableToFindWithCascade(newCascade);
			}
		}
	}
}



bool FaceDetection::TryToApplyFaceDetectorsAgain(cv::CascadeClassifier& cascade,  cv::Mat& Image, const std::string& cascadeName) {
	if (!LoadCascade(cascade, cascadeName))
		return false;
	cascade.detectMultiScale(Image, objbuffer, 1.1, 0, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
	if (objbuffer.empty())
		return false;
	else 
		return true;
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
	else if(objects.size() >2 ){ ///// what is possible num of eyes are 2+?
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