#include "timer.h"
#include "shutdownUtils.h"

Timer::Timer(std::string a_EndTime)
{
	_tzset();
	_get_timezone(&m_TimeZone);

	std::tm tm = {};
	std::stringstream ss(a_EndTime);

	time_t nowTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	localtime_s(&tm, &nowTime);
	ss >> std::get_time(&tm, "%H:%M");
	tm.tm_sec = 0;

	m_EndTime = std::chrono::system_clock::from_time_t(std::mktime(&tm) - m_TimeZone);
};

void Timer::StartTimer()
{
	using namespace std::chrono;
	// chrono::zoned_time works weird, so i'm using conversions
	system_clock::time_point nowTime = system_clock::from_time_t(system_clock::to_time_t(system_clock::now()) - m_TimeZone);
	duration<double> timeLeft = m_EndTime - nowTime;
	std::cout << std::format("Current time: {:%H:%M:%S %d.%m.%Y}, set timer for {:%H:%M:%S %d.%m.%Y}", nowTime, m_EndTime) << std::endl;
	
	if (timeLeft.count() > 0)
	{
		std::cout << std::format("Shutdown in {:%T}", timeLeft) << std::endl;
		std::this_thread::sleep_for(timeLeft);
	}

	wchar_t msg[1];
	SystemShutdown(msg, 60);
};