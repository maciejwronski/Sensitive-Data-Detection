#pragma once
#include <iostream>
#include <cstdlib>
#include "messages.h"
#include "File.h"
class Menu {
private:
	short _selectFileToProcess;
	short _selectTypeOfObject;
	short _selectTypeOfCensor;
	void SetMenu(short select);

public:
	short GetMenu() const;
	short GetTypeOfObjectToDetect();
	short GetTypeOfCensor();
	std::string GetPathToFileFromUser();
	Menu(const short choice) : _selectFileToProcess(choice) {};
};