#include "stdafx.h"
#include "menu.h"

void Menu::SetMenu(short select) {
	_selectFileToProcess = select;
}

short Menu::GetMenu() const {
	return _selectFileToProcess;
}

std::string Menu::GetPathToFileFromUser()
{
	std::cout << Messages::DefinePath();
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::string filePath;
	while ((std::getline(std::cin, filePath) && !File::Exists(filePath))) { /* Until valid filepath or enter pressed*/
		if (filePath == "") {
			std::cout << Messages::ApplicationExit();
			exit(1);
		}
		std::cout << Messages::FileDoesntExists(filePath);
	}
	std::cout << Messages::FileExists();

	return filePath;
}

short Menu::GetTypeOfObjectToDetect()
{
	std::cout << Messages::AskForTypeOfObject();
	while (!(std::cin >> _selectTypeOfObject) || _selectTypeOfObject > 2 || _selectTypeOfObject < 0) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << Messages::AskForTypeOfObject();
	}
	return _selectTypeOfObject;
}
