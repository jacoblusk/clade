#include "error.h"

void
WriteError(const LPWSTR lpszFmt, ...) {
    BOOL bResult;
    HANDLE hStdErr;
    WCHAR awcBuffer[1025];
    DWORD numberOfCharsWritten;
    va_list args;

    va_start(args, lpszFmt);
    wvsprintfW(awcBuffer, lpszFmt, args);
    va_end(args);

    /* Make sure we're null-terminated. */
    awcBuffer[1024] = '\0';
    hStdErr = GetStdHandle(STD_ERROR_HANDLE);
    bResult = WriteConsole(hStdErr, awcBuffer, lstrlenW(awcBuffer), &numberOfCharsWritten, NULL);
    if(!bResult) {
        /* What to do here? */
    }
}
