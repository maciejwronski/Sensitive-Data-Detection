#include "stdafx.h"
#include "menu.h"

void Menu::SetMenu(short select) {
	_selectFileToProcess = select;
}

short Menu::GetMenu() const {
	return _selectFileToProcess;
}

void Menu::GetPathFromUser(std::string& filePath)
{
	std::cout << Messages::DefinePath();
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	while ((std::getline(std::cin, filePath) && !File::Exists(filePath))) { /* Until valid filepath or enter pressed*/
		if (filePath == "") {
			std::cout << Messages::ApplicationExit();
			exit(1);
		}
		std::cout << Messages::FileDoesntExists();
	}
	
	std::cout << Messages::FileExists();
}