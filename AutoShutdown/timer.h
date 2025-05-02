#pragma once
#include <chrono>
#include <sstream>
#include <thread>
#include <format>	
#include <iostream>
#include <filesystem>

#include "logLib.h"

class Timer
{
private:
	const std::string INI_FILE = "AutoShutdown.ini";

	long m_TimeZone;
public:
	Timer();

	std::chrono::system_clock::time_point SetEndTime(std::string a_EndTime);
	std::string LoadEndTimeFromFile();
	void StartTimer();
	void StartTimer(std::string a_EndTime);
};