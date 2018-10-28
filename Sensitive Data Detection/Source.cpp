#include "stdafx.h"
#include <iostream>
#include <cstdlib>

#include "messages.h"
#include "menu.h"
#include "File.h"

#include "FaceDetection.h"
int main()
{
	std::cout << Messages::FileToProcess();
	short _typeChoice;
	while (!(std::cin >> _typeChoice) || _typeChoice > 2 || _typeChoice < 1) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << Messages::InvalidInput();
	}

	Menu menu{ _typeChoice };

	std::string _filePath;
	_filePath = menu.GetPathToFileFromUser();


	short _typeOfObjectToDetect;
	_typeOfObjectToDetect = menu.GetTypeOfObjectToDetect();

	short _typeOfCensor;
	_typeOfCensor = menu.GetTypeOfCensor();

	FaceDetection face{ _filePath };
	face.ShowObjects(_typeOfCensor);

	switch (_typeOfObjectToDetect) {

	case 0: break;
	case 1:	break;
	case 2:	break;
	}
    return 0;
}

