#include "file.hpp"
#include "logger.hpp"

std::string file::readFileAsString(const char* filename)
{
	std::string result = "";

	std::ifstream file(filename);
	if (file.fail()) {
		DebugConsole::error(std::string("File doesn't exist or cannot be accessed: \"" + std::string(filename) + "\"").c_str());
		file.close();
		return "\0";
	}

	std::string line;
	while (std::getline(file, line)) {
		result = result + line + "\n";
	}

	file.close();
	return result;
}