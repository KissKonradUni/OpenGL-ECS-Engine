#pragma once
#include <chrono>
#include <ctime>

// A simple wrapper class for logging messages.
namespace DebugConsole {

	// A simple informative message.
	void log(const char* text);
	// A message about an event that wasn't successful but didn't break the program.
	void warn(const char* text);
	// An error message which should be resolved.
	void error(const char* text);

	// Gets the current system time in a string format.
	std::string getCurrentTime();

};