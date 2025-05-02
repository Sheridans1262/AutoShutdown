#include "shutdownUtils.h"
#include "timer.h"

#include <regex>


int main(int argc, char** argv)
{
	setlocale(LC_ALL, "en_US.utf8");

	wchar_t progPath[1024];
	GetModuleFileName(NULL, progPath, 1024);
	HKEY hkey = NULL;
	LONG createStatus = RegCreateKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey); //Creates a key       
	LONG status = RegSetValueEx(hkey, L"AutoShutdown", 0, REG_SZ, (BYTE*)progPath, (wcslen(progPath) + 1) * sizeof(wchar_t));

	try
	{
		bool timeArgs = false;
		std::string endTime;
		if (argc > 1)
		{
			std::regex inputCheck("^(([0-1]\\d)|(2[0-3])):([0-5]\\d)$");
			if (!std::regex_search(argv[1], inputCheck))
				throw std::invalid_argument("Invalid time input.");
			endTime = argv[1];

			timeArgs = true;
		}

		Timer timer;
		if (timeArgs)
		{
			timer.StartTimer(endTime);
		}
		else
		{
			timer.StartTimer();
		}

		std::string line = "";
		std::getline(std::cin, line);
		if (line.length() != 0)
			PreventSystemShutdown();
	}
	catch (std::runtime_error& rEx)
	{
		logging::logmsg(rEx.what(), logging::ERRORLEVEL);
		std::cout << "Error: " << rEx.what() << std::endl;
	}
}
