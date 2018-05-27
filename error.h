#ifndef ERROR_H
#define ERROR_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include <windows.h>

void WriteError(const LPWSTR, ...);

#endif