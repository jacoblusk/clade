#ifndef HASHMAP_H
#define HASHMAP_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#define INITIAL_SIZE (256)
#define MAX_CHAIN_LENGTH (8)
#define MAP_MISSING -3
#define MAP_FULL -2
#define MAP_OMEM -1
#define MAP_OK 0

#include <windows.h>

typedef VOID (*PHASHMAP_MAPPROC)(LPVOID pUserData, LPVOID pValue);
typedef PHASHMAP_MAPPROC PHASHMAP_FREEDATAPROC;

typedef struct tagHASHMAP_ELEMENT HASHMAP_ELEMENT, *PHASHMAP_ELEMENT;

struct tagHASHMAP_ELEMENT {
    LPWSTR           m_lpszKey;
    LPVOID           m_pData;
    PHASHMAP_ELEMENT m_pNext;
};

typedef struct tagHASHMAP {
    SIZE_T                m_cTableSize;
    SIZE_T                m_cSize;
    PHASHMAP_ELEMENT      m_aElements;
    PHASHMAP_FREEDATAPROC m_pfnFreeDataProc;
    LPVOID                m_pUserData;
} HASHMAP, *PHASHMAP;

PHASHMAP HashMap_Create(LPVOID m_pUserData, PHASHMAP_FREEDATAPROC pfnFreeDataProc);
VOID HashMap_MapFn(PHASHMAP pHashmap, LPVOID pUserData, PHASHMAP_MAPPROC pfnMapProc);
VOID HashMap_Release(PHASHMAP *ppHashMap);
SIZE_T HashMap_Hash(LPWSTR lpszKey);
INT HashMap_Rehash(PHASHMAP pHashMap);
VOID HashMap_Put(PHASHMAP pHashMap, LPWSTR lpszKey, LPVOID pValue);
LPVOID HashMap_Get(PHASHMAP pHashMap, LPWSTR lpszKey);
INT HashMap_Remove(PHASHMAP pHashMap, LPWSTR lpszKey);

#endif