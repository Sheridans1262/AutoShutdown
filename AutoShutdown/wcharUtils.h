#pragma once

#include <array>
#include <stdexcept>
#include <windows.h>

// error C4996: 'wctomb': This function or variable may be unsafe. Consider using wctomb_s instead.
// To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS 

wchar_t* StrToWCHAR(std::string str);
std::string WCHARToStr(const std::wstring& wStr);
