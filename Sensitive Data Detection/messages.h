#pragma once
#include "stdafx.h"
#include <string>

static class Messages {
public:
	static const std::string FileToProcess() { return std::string("Would you like to process an image (1) or video (2)\n"); };
	static const std::string DefinePath() { return std::string("Please define Path to the file, either video or image\n"); };
	static const std::string InvalidInput() { return std::string("Invalid Input. Please Try Again\n"); };
	static const std::string FileExists() { return std::string("File Exists\n"); };
	static const std::string FileDoesntExists(const std::string& fileName) { return std::string("File with name " + fileName + " doesn't exists\n Please enter valid file name or press ENTER to EXIT\n"); };
	static const std::string ApplicationExit() { return std::string("Application Exit\n"); };
	static const std::string AskForTypeOfObject() { return std::string("Please enter type of object to detect. \n 0: Face \n 1: License Plate \n 2: Number of building\n "); }
	static const std::string CouldntLoadCascade(const std::string& cascadeName) { return std::string("Couldn't load cascade " +  cascadeName + " \n "); }
	static const std::string CouldntLoadImage(const std::string& imageName) { return std::string("Couldn't load image" + imageName + " \n "); }
	static const std::string UnableToFindWithCascade(const std::string& cascadeName) { return std::string("Couldn't find object by using " + cascadeName + " \n "); }
	static const std::string UsingNewCascade(const std::string& cascadeName) { return std::string("Using new cascade:  " + cascadeName + " \n "); }
	static const std::string FoundByCascade(const std::string& cascadeName) { return std::string("Found object by using:  " + cascadeName + " \n "); }
	static const std::string CouldntFindWithRotator() { return std::string("Couldnt find object with used rotator \n "); }
	static const std::string FoundWithRotator(const std::string& cascadeName) { return std::string("Found objects by using rotator on Cascadename " + cascadeName + "\n"); }
	static const std::string TryingWithRotator() { return std::string("Cascades are empty. Trying with rotator. \n "); }
	static const std::string FoundWithRotatorOnAppliedCascade(const std::string& cascadeName) { return std::string("Found another objects on main cascade using " + cascadeName + " \n "); }
};