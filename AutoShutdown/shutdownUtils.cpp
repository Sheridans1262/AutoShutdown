#include "shutdownUtils.h"; 

BOOL SystemShutdown(LPTSTR lpMsg, int seconds)
{
    HANDLE hToken;              // handle to process token 
    TOKEN_PRIVILEGES tkp;       // pointer to token structure 

    BOOL fResult;               // system shutdown flag 

    // Get the current process token handle so we can get shutdown 
    // privilege. 

    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return FALSE;

    // Get the LUID for shutdown privilege. 

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
        &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;  // one privilege to set    
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Get shutdown privilege for this process. 

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
        (PTOKEN_PRIVILEGES)NULL, 0);

    // Cannot test the return value of AdjustTokenPrivileges. 

    if (GetLastError() != ERROR_SUCCESS)
        return FALSE;

    // Display the shutdown dialog box and start the countdown. 

    fResult = InitiateSystemShutdownEx(
        NULL,    // shut down local computer 
        lpMsg,   // message for user
        seconds,      // time-out period, in seconds 
        TRUE,   // dont ask user to close apps 
        FALSE,  // dont reboot after shutdown 
        SHTDN_REASON_MAJOR_OTHER | 
        SHTDN_REASON_MINOR_OTHER | 
        SHTDN_REASON_FLAG_PLANNED); // reason for shutdown - other, planned   

    if (!fResult)
        return FALSE;

    // Disable shutdown privilege. 

    tkp.Privileges[0].Attributes = 0;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
        (PTOKEN_PRIVILEGES)NULL, 0);

    return TRUE;
}

BOOL PreventSystemShutdown()
{
    HANDLE hToken;              // handle to process token 
    TOKEN_PRIVILEGES tkp;       // pointer to token structure 

    // Get the current process token handle  so we can get shutdown 
    // privilege. 

    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return FALSE;

    // Get the LUID for shutdown privilege. 

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
        &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;  // one privilege to set    
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Get shutdown privilege for this process. 

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
        (PTOKEN_PRIVILEGES)NULL, 0);

    if (GetLastError() != ERROR_SUCCESS)
        return FALSE;

    // Prevent the system from shutting down. 

    if (!AbortSystemShutdown(NULL))
        return FALSE;

    // Disable shutdown privilege. 

    tkp.Privileges[0].Attributes = 0;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
        (PTOKEN_PRIVILEGES)NULL, 0);

    return TRUE;
}