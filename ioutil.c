#include "ioutil.h"

VOID
PrintfA2W(const LPSTR lpszFmt, ...) {
    HANDLE hStdOut;
    va_list args;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    va_start(args, lpszFmt);
    WriteVA2W(hStdOut, lpszFmt, args);
    va_end(args);
}

VOID
WriteVA2W(HANDLE hStdHandle, const LPSTR lpszFmt, va_list args) {
    CHAR acBuffer[1025];
    WCHAR awcBuffer[1025];
    DWORD numberOfCharsWritten;

    wvsprintfA(acBuffer, lpszFmt, args);

    acBuffer[1024] = '\0';
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, acBuffer, -1, awcBuffer, 1024);
    WriteConsole(hStdHandle, awcBuffer, lstrlenW(awcBuffer), &numberOfCharsWritten, NULL);
}

VOID
ErrorfW(const LPWSTR lpszFmt, ...) {
    HANDLE hStdErr;
    va_list args;

    hStdErr = GetStdHandle(STD_ERROR_HANDLE);
    va_start(args, lpszFmt);
    WriteVW(hStdErr, lpszFmt, args);
    va_end(args);
}

VOID
PrintfW(const LPWSTR lpszFmt, ...) {
    HANDLE hStdOut;
    va_list args;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    va_start(args, lpszFmt);
    WriteVW(hStdOut, lpszFmt, args);
    va_end(args);
}

VOID
WriteW(HANDLE hStdHandle, const LPWSTR lpszFmt, ...) {
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

VOID
WriteVW(HANDLE hStdHandle, const LPWSTR lpszFmt, va_list args) {
    WCHAR awcBuffer[1025];
    DWORD numberOfCharsWritten;

    wvsprintfW(awcBuffer, lpszFmt, args);

    awcBuffer[1024] = '\0';
    WriteConsole(hStdHandle, awcBuffer, lstrlenW(awcBuffer), &numberOfCharsWritten, NULL);
}

VOID
ErrorfA(const LPSTR lpszFmt, ...) {
    #ifndef DEBUG
    HANDLE hStdErr;
    va_list args;

    hStdErr = GetStdHandle(STD_ERROR_HANDLE);
    va_start(args, lpszFmt);
    WriteVA(hStdErr, lpszFmt, args);
    va_end(args);
    #endif
}

VOID
PrintfA(const LPSTR lpszFmt, ...) {
    HANDLE hStdOut;
    va_list args;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    va_start(args, lpszFmt);
    WriteVA(hStdOut, lpszFmt, args);
    va_end(args);
}

VOID
WriteWA(HANDLE hStdHandle, const LPSTR lpszFmt, ...) {
    CHAR awcBuffer[1025];
    DWORD numberOfCharsWritten;
    va_list args;

    va_start(args, lpszFmt);
    wvsprintfA(awcBuffer, lpszFmt, args);
    va_end(args);

    /* Make sure we're null-terminated. */
    awcBuffer[1024] = '\0';
    WriteConsole(hStdHandle, awcBuffer, lstrlenA(awcBuffer), &numberOfCharsWritten, NULL);
}

VOID
WriteVA(HANDLE hStdHandle, const LPSTR lpszFmt, va_list args) {
    CHAR awcBuffer[1025];
    DWORD numberOfCharsWritten;

    wvsprintfA(awcBuffer, lpszFmt, args);

    awcBuffer[1024] = '\0';
    WriteConsole(hStdHandle, awcBuffer, lstrlenA(awcBuffer), &numberOfCharsWritten, NULL);
}