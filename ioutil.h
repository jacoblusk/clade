#ifndef IOUTIL_H
#define IOUTIL_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include <windows.h>

void Errorf(const LPWSTR, ...);
void Printf(const LPWSTR, ...);
void Write(HANDLE nStdHandle, const LPWSTR lpszFmt, ...);
void WriteV(HANDLE nStdHandle, const LPWSTR lpszFmt, va_list args);

#endif