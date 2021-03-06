#include "stdafx.h"
#include <iostream>
#include <cstdlib>

#include "messages.h"
#include "menu.h"
#include "File.h"
#include "LicensePlate.h"
#include "FaceDetection.h"

#define FOLDERS 0 // define if do you want detect whole folders
#define TYPE 0 // if FOLDERS == 0 *** 0 -- faces // 1 -- license plates // define in objectdetection.h or licenseplate.h 1 according to number given there

#if FOLDERS
#include <experimental/filesystem>
	#if !TYPE
		const std::string FolderPath { "C:\\Users\\Maciej\\Documents\\Visual Studio 2017\\Projects\\Sensitive Data Detection\\Sensitive Data Detection\\Sensitive Data Detection\\face_test_images\\images" };
	#else
		const std::string FolderPath { "C:\\Users\\Maciej\\Documents\\Visual Studio 2017\\Projects\\Sensitive Data Detection\\Sensitive Data Detection\\Sensitive Data Detection\\cars_test_images\\eu" };
	#endif
#endif

#if FOLDERS
std::vector<std::string> get_filenames(std::experimental::filesystem::path path)
{
	namespace stdfs = std::experimental::filesystem;

	std::vector<std::string> filenames;
	const stdfs::directory_iterator end{};

	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
	{
		if (stdfs::is_regular_file(*iter))
			filenames.push_back(iter->path().string());
	}

	return filenames;
}
#endif

int main()
{
	short _typeChoice = 0;
	Menu menu{ _typeChoice };
#if FOLDERS
	for (const auto& name : get_filenames(FolderPath)) {
		std::string _filePath = name;
#else
		std::string _filePath;
		_filePath = menu.GetPathToFileFromUser();
#endif
#if !FOLDERS
		short _typeOfObjectToDetect = 0;
#else 
		short _typeOfObjectToDetect = TYPE;
#endif
#if !FOLDERS
		_typeOfObjectToDetect = menu.GetTypeOfObjectToDetect();
#endif
		short _typeOfCensor = 1;
#if !FOLDERS
		_typeOfCensor = menu.GetTypeOfCensor();
#endif
		FaceDetection face{ _filePath };
		LicensePlateDetection licensePlateDetection{_filePath};
		try {
			switch (_typeOfObjectToDetect) {
			case 0: face.ShowObjects(_typeOfCensor); break;
			case 1: licensePlateDetection.ShowObjects(_typeOfCensor); break;
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		catch (const cv::Exception& e)
		{
			std::cout << e.what() << std::endl;
		}
#if FOLDERS
	}
#endif
    return 0;
}

