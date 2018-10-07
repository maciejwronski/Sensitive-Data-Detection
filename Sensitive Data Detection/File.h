#pragma once
#include <string>
#include <fstream>

static class File {
public:
	static bool Exists(const std::string& fileName);
};