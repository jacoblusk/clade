#ifndef HASHMAP_H
#define HASHMAP_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#define INITIAL_SIZE (256)
#define MAX_CHAIN_LENGTH (8)

#include <windows.h>

typedef struct tagHASHMAP_ELEMENT {
    LPWSTR m_szKey;
    SIZE_T m_inUse;
    LPVOID m_pData;
} HASHMAP_ELEMENT, *PHASHMAP_ELEMENT;

typedef struct tagHASHMAP {
    SIZE_T            m_cTableSize;
    SIZE_T            m_cSize;
    PHASHMAP_ELEMENT *m_aElements; 
} HASHMAP, *PHASHMAP;

PHASHMAP CreateHashMap(void);
void ReleaseHashMap(PHASHMAP *ppHashMap);

#endif