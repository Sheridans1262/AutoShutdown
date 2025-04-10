#pragma once
#include <chrono>
#include <sstream>
#include <thread>
#include <format>	
#include <iostream>

class Timer
{
private:
	std::chrono::system_clock::time_point m_EndTime;
	long m_TimeZone;
public:
	Timer(std::string a_EndTime);
	void StartTimer();
};