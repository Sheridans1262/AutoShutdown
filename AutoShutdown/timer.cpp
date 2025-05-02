#include "timer.h"
#include "shutdownUtils.h"
#include "wcharUtils.h"

Timer::Timer()
{
	_tzset();
	_get_timezone(&m_TimeZone);
	logging::logmsg("Timer instance initialized");
}


std::chrono::system_clock::time_point Timer::SetEndTime(std::string a_EndTime)
{
	std::tm tm = {};
	std::stringstream ss(a_EndTime);

	time_t nowTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	localtime_s(&tm, &nowTime);
	ss >> std::get_time(&tm, "%H:%M");
	tm.tm_sec = 0;

	return std::chrono::system_clock::from_time_t(std::mktime(&tm) - m_TimeZone);
}

void Timer::StartTimer()
{
	std::string endTime = LoadEndTimeFromFile();
	StartTimer(endTime);
}

void Timer::StartTimer(std::string a_EndTime)
{
	logging::logmsg("Begin StartTimer");
	using namespace std::chrono;
	system_clock::time_point endTime = SetEndTime(a_EndTime);

	// chrono::zoned_time works weird, so i'm using conversions
	system_clock::time_point nowTime = system_clock::from_time_t(system_clock::to_time_t(system_clock::now()) - m_TimeZone);
	duration<double> timeLeft = endTime - nowTime;
	std::cout << std::format("Current time: {:%H:%M:%S %d.%m.%Y}, set timer for {:%H:%M:%S %d.%m.%Y}", nowTime, endTime) << std::endl;
	logging::logmsg(std::format("Current time: {:%H:%M:%S %d.%m.%Y}, set timer for {:%H:%M:%S %d.%m.%Y}", nowTime, endTime));
	
	if (timeLeft.count() > 0)
	{
		std::cout << std::format("Shutdown in {:%T}", timeLeft) << std::endl;
		logging::logmsg(std::format("Shutdown in {:%T}", timeLeft));

		std::this_thread::sleep_for(seconds(5));
		ShowWindow(GetConsoleWindow(), SW_HIDE);

		duration<double> fiveSec = seconds(5);
		std::this_thread::sleep_for(timeLeft - fiveSec);
	}

	wchar_t msg[1];
	SystemShutdown(msg, 60);
	logging::logmsg("End StartTimer");
}

std::string Timer::LoadEndTimeFromFile()
{
	logging::logmsg("Start LoadEndTimeFromFile");
	try
	{
		if (!std::filesystem::exists(INI_FILE))
		{
			throw std::runtime_error(std::format("Settings file {} does not exists.", INI_FILE));
		}
		wchar_t* wstrSettings = StrToWCHAR(".\\" + INI_FILE);
		wchar_t* wstrReturnedString = new wchar_t[255];

		std::tm tm = {};
		time_t nowTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		localtime_s(&tm, &nowTime);
		if (tm.tm_wday == 5 || tm.tm_wday == 6)
		{
			GetPrivateProfileString(L"PREFERENCES", L"WeekendShutdownTime", L"23:00", wstrReturnedString, 255, wstrSettings);
		}
		else
		{
			GetPrivateProfileString(L"PREFERENCES", L"WeekdayShutdownTime", L"21:30", wstrReturnedString, 255, wstrSettings);
		}

		std::string result = WCHARToStr(wstrReturnedString);
		logging::logmsg("Time extracted from .ini file: " + result);
		return result;
	}
	catch (std::exception& ex)
	{
		std::cout << "Error: " << ex.what() << std::endl;
		logging::logmsg(ex.what(), logging::ERRORLEVEL);

		return "22:00";
	}
}