#pragma once
#include <iostream>
#include <cstdlib>
#include "messages.h"
#include "File.h"
class Menu {
private:
	short _selectFileToProcess;
	void SetMenu(short select);

public:
	short GetMenu() const;
	void GetPathFromUser(std::string& filePath);
	Menu(const short choice) : _selectFileToProcess(choice) {};
};