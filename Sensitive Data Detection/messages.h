#pragma once
#include "stdafx.h"
#include <string>

static class Messages {
public:
	static const std::string FileToProcess() { return std::string("Would you like to process an image (1) or video (2)\n"); };
	static const std::string DefinePath() { return std::string("Please define Path to the file, either video or image\n"); };
	static const std::string InvalidInput() { return std::string("Invalid Input. Please Try Again\n"); };
	static const std::string FileExists() { return std::string("File Exists\n"); };
	static const std::string FileDoesntExists() { return std::string("File doesn't exists\n Please enter valid file name or press ENTER to EXIT\n"); };
	static const std::string ApplicationExit() { return std::string("Application Exit\n"); };
};