#include "stdafx.h"
#include "FaceDetection.h"


// TODO: Image rotated by 180* on detail-based, Apply opposite rotation / what if face is rotated by 90+ / 269- degrees on detail-based 
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
			tempImage2 = ReturnImageWithMostPossibleObjects(cascade, tempImage, 30);
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
						cv::Mat tempImage = tempImage2;
						for (int i = 1; i < 3; i++) {
							std::cout << Messages::ApplyingNextCascade(cascadeVector[i]);
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
				else {
					return matFile;
				}
			}
		}
		objbuffer1.insert(std::end(objbuffer1), std::begin(objbuffer2), std::end(objbuffer2));
		objbuffer1.insert(std::end(objbuffer1), std::begin(objbuffer3), std::end(objbuffer3));
	}
	else if (MethodToFindRotation == ByFindingDetail) {
			cv::Mat tempImage = matFile;


			/*/ Let's be sure, that we can't find face right now */
			if (!LoadCascade(cascade, _mainCascade))
				return tempImage;
			cascade.detectMultiScale(tempImage, objbuffer1, 1.1, 4, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
			if (!LoadCascade(cascade, _profileCascadeName))
				return tempImage;
			cascade.detectMultiScale(tempImage, objbuffer2, 1.1, 4, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
			
			if (objbuffer1.size() != 0 || objbuffer2.size() != 0) {
				if (!LoadCascade(cascade, _eyeCascadeName))
					return tempImage;
				cascade.detectMultiScale(tempImage, objbuffer3, 1.1, 1, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
				return tempImage;
			}
			if (!LoadCascade(cascade, _eyeCascadeName))
				return matFile;
			cascade.detectMultiScale(tempImage, objbuffer3, 1.1, 4, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
			if (!objbuffer3.empty()) {
				if (objbuffer3.size() > 2) {
					FindTwoClosestRectangles(objbuffer3);
				}
				cv::Mat tempImage2 = tempImage;
				//Censor censor(Censor::FilledRect);
				//censor.SetFilledRect(objbuffer3, tempImage2, cv::Scalar(255, 0, 0));
//			cv::imshow("Detected Eyes", tempImage2);
				std::cout << Messages::FoundByCascade(_eyeCascadeName);


				cv::Point2f MiddleOfTwoPoints;
				float Degrees = CheckRotationByFindingDetail(tempImage, objbuffer3, MiddleOfTwoPoints);
				std::cout << "Rotating image by: " << Degrees << " \n";
				tempImage = RotateImage(matFile, Degrees, MiddleOfTwoPoints);

				/*After rotating Apply */
				for (int i = 0; i < 2; ++i) {
					if (!LoadCascade(cascade, cascadeVector[i]))
						return matFile;
					if(cascadeVector[i] == _mainCascade)
						cascade.detectMultiScale(tempImage, objbuffer1, 1.1, 4, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
					else
						cascade.detectMultiScale(tempImage, objbuffer2, 1.1, 4, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));

					float tryToRotateAroundDegrees = 90;
					if (cascadeVector[i] == _mainCascade) {
						while (objbuffer1.size() == 0 && tryToRotateAroundDegrees < 360) {
							tempImage = RotateImage(tempImage, tryToRotateAroundDegrees);
							cascade.detectMultiScale(tempImage, objbuffer1, 1.1, 4, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
							tryToRotateAroundDegrees += 90;
							std::cout << tryToRotateAroundDegrees << std::endl;
						}
					}

					else {
						while (objbuffer2.size() == 0 || tryToRotateAroundDegrees < 360) {
							tempImage = RotateImage(tempImage, tryToRotateAroundDegrees);
							cascade.detectMultiScale(tempImage, objbuffer2, 1.1, 4, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
							tryToRotateAroundDegrees += 90;
						}
					}

					/* Apply eye cascade again to find eyes on rotated image */
					if (!LoadCascade(cascade, _eyeCascadeName))
						return matFile;
					objbuffer3.clear();
					cascade.detectMultiScale(tempImage, objbuffer3, 1.1, 4, 0, cv::Size(minWidth, minHeight), cv::Size(maxWidth, maxHeight));
					return tempImage;
				}
			}
			else {
				std::cout << Messages::UnableToFindWithCascade(_eyeCascadeName);
				std::cout << "Applying Rotation method " << std::endl;
				MethodToFindRotation = ByRotatingImage;
				matFile = DetectObjects(_cascade, matFile, objbuffer1, objbuffer2, objbuffer3, minWidth, minHeight, maxWidth, maxHeight);
				MethodToFindRotation = ByFindingDetail;
				return matFile;
			}
		}
}


int FaceDetection::CheckRotationByFindingDetail(const cv::Mat& originalImage, std::vector<cv::Rect>& objects, cv::Point2f& Middle) {
	if (objects.size() == 2) {
		cv::Mat img;
		img = originalImage;
		SetWidthAndHeightSame(objects[0], objects[1]);
		std::vector<cv::Point> points(2);

		points[0] = objects[0].tl();
		points[1] = objects[1].tl();
		cv::line(img, points[0], points[1], cv::Scalar(255, 0, 0));


		Middle = CalculateMiddleOfTwoPoints(points[0], points[1]);


		float len = sqrt(pow(points[0].x - points[1].x, 2) + pow(points[0].y - points[1].y, 2));
		float a = points[0].x - points[1].x;

		return FindRemainingAngle(points[0], points[1], Middle) * Rad2Deg(cos(a / len));
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
		for (int j = 0; j < vec.size(); j++) {
			if (i == j)
				continue;
			tempDist = sqrt(pow(vecbr[i].x - vectl[j].x, 2) + pow(vecbr[i].y - vectl[j].y, 2));
			if (tempDist < currMinDistance) {
				currMinDistance = tempDist;
				indexes[0] = i;
				indexes[1] = j;
				//std::cout << indexes[0] << " " << indexes[1] << std::endl;
			}
		}
	}
	for (int i = 0; i < vec.size(); i++) {
		if (i == indexes[0] || i == indexes[1])
			continue;
		else {
			vec.erase(vec.begin() + i);
		}
	}
}

int FaceDetection::FindRemainingAngle(const cv::Point2f& point1, const cv::Point2f& point2, const cv::Point& axisPoint) {

	if (point1.x < point2.x && point1.y > point2.y)
		return -1;
	else return 1;
}

void FaceDetection::ShowObjects(int  censorType)
{
	if (!LoadImage(matFile, _filePath) || !LoadCascade(_cascade, _cascadeName))
		return;
	CreateWindow(_windowName);
//	cv::imshow("Original Image", matFile);
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
#if !FOLDERS
	cv::imshow(_windowName, matFile);
	cv::waitKey(0);
#else
	std::string name{ std::to_string(counter) + ".jpg" };
	std::string all = ResultPath + name;
	cv::imwrite(all, matFile);
	counter++;
#endif
	//cv::destroyAllWindows();
}