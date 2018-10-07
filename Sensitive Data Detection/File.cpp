#pragma once
#include "stdafx.h"
#include "File.h"

bool File::Exists(const std::string& fileName){
	std::ifstream ifile(fileName);
	return ifile.good();
}