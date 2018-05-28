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

typedef struct tagHASHMAP_ELEMENT {
    LPWSTR m_lpszKey;
    BOOL m_bInUse;
    LPVOID m_pData;
} HASHMAP_ELEMENT, *PHASHMAP_ELEMENT;

typedef struct tagHASHMAP {
    SIZE_T            m_cTableSize;
    SIZE_T            m_cSize;
    PHASHMAP_ELEMENT  m_aElements; 
} HASHMAP, *PHASHMAP;

PHASHMAP HashMap_Create(void);
void HashMap_Release(PHASHMAP *ppHashMap);
INT HashMap_Rehash(PHASHMAP pHashMap);
INT HashMap_Put(PHASHMAP pHashMap, LPWSTR lpszKey, LPVOID pValue);
INT HashMap_Get(PHASHMAP pHashMap, LPWSTR lpszKey, LPVOID *ppValue);
INT HashMap_Remove(PHASHMAP pHashMap, LPWSTR lpszKey);

#endif