#include "shutdownUtils.h"
#include "timer.h"

#include <regex>


int main(int argc, char** argv)
{
	setlocale(LC_ALL, "en_US.utf8");

	std::string endTime;
	if (argc > 1)
	{
		try
		{
			std::regex inputCheck("^(([0-1]\\d)|(2[0-3])):([0-5]\\d)$");
			if (!std::regex_search(argv[1], inputCheck))
				throw std::invalid_argument("Invalid time input.");
			endTime = argv[1];
		}
		catch (std::exception& ex)
		{
			endTime = "22:00";
		}
	}
	else
	{
		endTime = "22:00";
	}

	try
	{
		Timer timer(endTime);
		timer.StartTimer();
		std::string line = "";
		std::getline(std::cin, line);
		if (line.length() != 0)
			PreventSystemShutdown();
	}
	catch (std::runtime_error& rEx)
	{
		std::cout << "Error: " << rEx.what() << std::endl;
		//logmsg(rEx.what(), LogLevel::ERRORLEVEL);
		//LOG(ERROR) << rEx.what();
	}
	//wchar_t progPath[1024];
	//GetModuleFileName(NULL, progPath, 1024);
	//HKEY hkey = NULL;
	//LONG createStatus = RegCreateKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey); //Creates a key       
	//LONG status = RegSetValueEx(hkey, L"AutoShutdown", 0, REG_SZ, (BYTE*)progPath, (wcslen(progPath) + 1) * sizeof(wchar_t));
}