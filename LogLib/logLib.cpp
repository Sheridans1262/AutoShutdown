#include "logLib.h"

namespace logging
{
    void logmsg(std::string message, LogLevel level)
    {
        std::ofstream file(LOG_FILENAME, std::ios::app);
        if (file.is_open())
        {
            std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm tm = {};
            localtime_s(&tm, &time);
            file << std::put_time(&tm, "%F %T - ");
            file << level << ": " << message << "\n";
            file.close();
        }
    }
}