// Sensitive Data Detection.cpp: definiuje punkt wejścia dla aplikacji konsolowej.
//
#include <opencv2/opencv.hpp>

#include "stdafx.h"
#include <iostream>
#include <cstdlib>

#include "messages.h"
#include "menu.h"
#include "File.h"

int main()
{
	std::cout << Messages::FileToProcess();

	short typeChoice;
	while (!(std::cin >> typeChoice) || typeChoice > 2 || typeChoice < 1) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << Messages::InvalidInput();
	}

	Menu menu{ typeChoice };

	std::string _filePath;
	menu.GetPathFromUser(_filePath);

    return 0;
}

