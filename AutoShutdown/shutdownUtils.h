#pragma once
#include <windows.h>

#pragma comment( lib, "advapi32.lib" )

BOOL SystemShutdown(LPTSTR lpMsg, int seconds);
BOOL PreventSystemShutdown();
