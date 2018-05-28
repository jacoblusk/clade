#include "hashmap.h"

PHASHMAP CreateHashMap(void) {
    PHASHMAP pHashMap;
    HANDLE hProcessHeap;

    hProcessHeap = GetProcessHeap();
    pHashMap = HeapAlloc(hProcessHeap, 0, sizeof(HASHMAP));
    if(!pHashMap) {
        return NULL;
    }

    pHashMap->m_aElements = HeapAlloc(hProcessHeap, HEAP_ZERO_MEMORY, sizeof(HASHMAP_ELEMENT) * INITIAL_SIZE);
    if(!pHashMap->m_aElements) {
        BOOL bResult;

        bResult = HeapFree(hProcessHeap, 0, pHashMap);
        if(!bResult) {
            /*TODO: do something here*/
        }
        return NULL;
    }

    pHashMap->m_cTableSize = INITIAL_SIZE;
    pHashMap->m_cSize = 0;

    return pHashMap;
}

void ReleaseHashMap(PHASHMAP *ppHashMap) {
    HANDLE hProcessHeap;

    if(*ppHashMap && (*ppHashMap)->m_aElements) {
        hProcessHeap = GetProcesHeap();
        HeapFree(hProcessHeap, 0, (*ppHashMap)->m_aElements);
        (*ppHashMap)->m_aElements = NULL;

        HeapFree(hProcessHeap, 0, (*ppHashMap)->m_aElements);
        *ppHashMap = NULL;
    }
}