#ifndef IOUTIL_H
#define IOUTIL_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include <windows.h>

VOID PrintfA2W(const LPSTR, ...);
VOID WriteVA2W(HANDLE nStdHandle, const LPSTR lpszFmt, va_list args);

VOID ErrorfW(const LPWSTR, ...);
VOID PrintfW(const LPWSTR, ...);
VOID WriteW(HANDLE nStdHandle, const LPWSTR lpszFmt, ...);
VOID WriteVW(HANDLE nStdHandle, const LPWSTR lpszFmt, va_list args);

VOID ErrorfA(const LPSTR, ...);
VOID PrintfA(const LPSTR, ...);
VOID WriteA(HANDLE nStdHandle, const LPSTR lpszFmt, ...);
VOID WriteVA(HANDLE nStdHandle, const LPSTR lpszFmt, va_list args);

#endif