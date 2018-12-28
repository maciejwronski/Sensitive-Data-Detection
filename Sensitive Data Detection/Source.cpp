#include "stdafx.h"
#include <iostream>
#include <cstdlib>

#include "messages.h"
#include "menu.h"
#include "File.h"

#include "LicensePlate.h"
#include "FaceDetection.h"
int main()
{
	short _typeChoice = 0;

	Menu menu{ _typeChoice };

	std::string _filePath;
	_filePath = menu.GetPathToFileFromUser();


	short _typeOfObjectToDetect;
	_typeOfObjectToDetect = menu.GetTypeOfObjectToDetect();

	short _typeOfCensor;
	_typeOfCensor = menu.GetTypeOfCensor();
	FaceDetection face{ _filePath };
	LicensePlateDetection licensePlateDetection{ _filePath };
	switch (_typeOfObjectToDetect) {
		case 0: face.ShowObjects(_typeOfCensor); break;
		case 1: licensePlateDetection.ShowObjects(_typeOfCensor); break;
	}
    return 0;
}

