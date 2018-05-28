#include "ioutil.h"

void
Errorf(const LPWSTR lpszFmt, ...) {
    HANDLE hStdErr;
    va_list args;

    hStdErr = GetStdHandle(STD_ERROR_HANDLE);
    va_start(args, lpszFmt);
    WriteV(hStdErr, lpszFmt, args);
    va_end(args);
}

void
Printf(const LPWSTR lpszFmt, ...) {
    HANDLE hStdOut;
    va_list args;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    va_start(args, lpszFmt);
    WriteV(hStdOut, lpszFmt, args);
    va_end(args);
}

void
Write(HANDLE hStdHandle, const LPWSTR lpszFmt, ...) {
    WCHAR awcBuffer[1025];
    DWORD numberOfCharsWritten;
    va_list args;

    va_start(args, lpszFmt);
    wvsprintfW(awcBuffer, lpszFmt, args);
    va_end(args);

    /* Make sure we're null-terminated. */
    awcBuffer[1024] = '\0';
    WriteConsole(hStdHandle, awcBuffer, lstrlenW(awcBuffer), &numberOfCharsWritten, NULL);
}

void
WriteV(HANDLE hStdHandle, const LPWSTR lpszFmt, va_list args) {
    WCHAR awcBuffer[1025];
    DWORD numberOfCharsWritten;

    wvsprintfW(awcBuffer, lpszFmt, args);

    awcBuffer[1024] = '\0';
    WriteConsole(hStdHandle, awcBuffer, lstrlenW(awcBuffer), &numberOfCharsWritten, NULL);
}