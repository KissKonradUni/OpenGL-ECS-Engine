#pragma once
#include "includes.hpp"

#include <vector>
#include <iostream>
#include <fstream>

namespace file {
	// Reads a whole file as one std::string with linebreaks at each line. (Uses \n only.)
	std::string readFileAsString(const char* filename);
}