#pragma once

#include "time.h"
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>

namespace logging 
{
    enum LogLevel
    {
        INFOLEVEL,
        WARNINGLEVEL,
        ERRORLEVEL,
    };

    const std::string LOG_FILENAME = "logfile.log";
    void logmsg(std::string message, LogLevel level = LogLevel::INFOLEVEL);

}
