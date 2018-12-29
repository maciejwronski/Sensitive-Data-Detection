#include "stdafx.h"
#include <iostream>
#include <cstdlib>

#include "messages.h"
#include "menu.h"
#include "File.h"

#include "LicensePlate.h"
#include "FaceDetection.h"

#include <experimental/filesystem>

std::vector<std::string> get_filenames(std::experimental::filesystem::path path)
{
	namespace stdfs = std::experimental::filesystem;

	std::vector<std::string> filenames;

	// http://en.cppreference.com/w/cpp/experimental/fs/directory_iterator
	const stdfs::directory_iterator end{};

	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
	{
		// http://en.cppreference.com/w/cpp/experimental/fs/is_regular_file 
		if (stdfs::is_regular_file(*iter)) // comment out if all names (names of directories tc.) are required
			filenames.push_back(iter->path().string());
	}

	return filenames;
}


int main()
{
	short _typeChoice = 0;
	Menu menu{ _typeChoice };
	for (const auto& name : get_filenames("C:\\Users\\Maciej\\Documents\\Visual Studio 2017\\Projects\\Sensitive Data Detection\\Sensitive Data Detection\\Sensitive Data Detection\\cars_test_images\\eu")) {
		std::string _filePath;
		//_filePath = menu.GetPathToFileFromUser();


		short _typeOfObjectToDetect = 1;
		//_typeOfObjectToDetect = menu.GetTypeOfObjectToDetect();

		short _typeOfCensor = 1;
		//_typeOfCensor = menu.GetTypeOfCensor();
		FaceDetection face{ _filePath };
		LicensePlateDetection licensePlateDetection{/* _filePath*/ name };
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
	}
    return 0;
}

