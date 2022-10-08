#include "includes.hpp"
#include "logger.hpp"

const char* BlackFr   = "\u001b[30m";
const char* RedFr     = "\u001b[31m";
const char* GreenFr   = "\u001b[32m";
const char* YellowFr  = "\u001b[33m";
const char* BlueFr	  = "\u001b[34m";
const char* MagentaFr = "\u001b[35m";
const char* CyanFr    = "\u001b[36m";
const char* WhiteFr	  = "\u001b[37m";
const char* ResetFr   = "\u001b[0m";

namespace DebugConsole {

	void log(const char* text)
	{
		std::cout << "[" << CyanFr << "INF" << ResetFr << "][" << getCurrentTime() << "] - " << text << std::endl;
	}

	void warn(const char* text)
	{
		std::cout << "[" << YellowFr << "WRN" << ResetFr << "][" << getCurrentTime() << "] - " << text << std::endl;
	}

	void error(const char* text)
	{
		std::cout << "[" << RedFr << "ERR" << ResetFr << "][" << getCurrentTime() << "] - " << text << std::endl;
	}

	// Hardcoded 32 length cstring for getting the current time
	std::string getCurrentTime()
	{
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string buf;
		char* buffer = new char[32];
		ctime_r(&time, &buffer[0]);
		if (buffer[strlen(buffer) - 1] == '\n') buffer[strlen(buffer) - 1] = '\0';
		buf += buffer;
		delete[] buffer;

		return buf;
	}

}