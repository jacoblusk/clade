#include "../ioutil.h"
#include "hashmap.h"

PHASHMAP
HashMap_Create(void) {
    PHASHMAP pHashMap;
    HANDLE hProcessHeap;

    hProcessHeap = GetProcessHeap();
    pHashMap = HeapAlloc(hProcessHeap, 0, sizeof(HASHMAP));
    if(!pHashMap) {
        return NULL;
    }

    pHashMap->m_aElements = HeapAlloc(hProcessHeap, HEAP_ZERO_MEMORY, sizeof(HASHMAP_ELEMENT) * INITIAL_SIZE);
    if(!pHashMap->m_aElements) {
        HeapFree(hProcessHeap, 0, pHashMap);
        return NULL;
    }

    pHashMap->m_cTableSize = INITIAL_SIZE;
    pHashMap->m_cSize = 0;

    return pHashMap;
}

void
HashMap_Release(PHASHMAP *ppHashMap) {
    HANDLE hProcessHeap;

    if(*ppHashMap && (*ppHashMap)->m_aElements) {
        hProcessHeap = GetProcessHeap();
        HeapFree(hProcessHeap, 0, (*ppHashMap)->m_aElements);
        (*ppHashMap)->m_aElements = NULL;

        HeapFree(hProcessHeap, 0, (*ppHashMap)->m_aElements);
        *ppHashMap = NULL;
    }
}

SIZE_T
HashMap_Hash(LPWSTR lpszKey) {
    SIZE_T hash = 5381;
    SIZE_T c;

    while ((c = *(lpszKey++)))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

static LPWSTR __HashMap_LPWSTRCopy(LPWSTR lpszString) {
    HANDLE hProcessHeap;
    LPWSTR lpszCopy;

    hProcessHeap = GetProcessHeap();
    lpszCopy = HeapAlloc(hProcessHeap, 0, sizeof(WCHAR) * (lstrlenW(lpszString) + 1));
    if(!lpszCopy)
        return NULL;

    lstrcpyW(lpszCopy, lpszString);
    return lpszCopy;
}

void HashMap_Put(PHASHMAP pHashMap, LPWSTR lpszKey, LPVOID pValue) {
    SIZE_T hash;
    HANDLE hProcessHeap;
    PHASHMAP_ELEMENT pElement;
    
    hProcessHeap = GetProcessHeap();
    hash = HashMap_Hash(lpszKey);
    pElement = &(pHashMap->m_aElements[hash % pHashMap->m_cTableSize]);
    if(!pElement->m_pData) {
        LPWSTR lpszKeyCopy = NULL;

        lpszKeyCopy = __HashMap_LPWSTRCopy(lpszKey);
        if(!lpszKeyCopy) {
            Errorf(L"error: __HashMap_LPWSTRCopy failed.\n");
            return;
        }

        pElement->m_pData = pValue;
        pElement->m_lpszKey = lpszKeyCopy;
    } else {
        PHASHMAP_ELEMENT pNewElement;

        pNewElement = HeapAlloc(hProcessHeap, HEAP_ZERO_MEMORY, sizeof(HASHMAP_ELEMENT));
        if(!pNewElement) {
            Errorf(L"error: HeapAlloc failed for HashMap_Put (pNewElement).\n");
            return;
        }

        pNewElement->m_lpszKey = __HashMap_LPWSTRCopy(lpszKey);
        if(!pNewElement->m_lpszKey) {
            Errorf(L"error: __HashMap_LPWSTRCopy failed.\n");
            HeapFree(hProcessHeap, 0, pNewElement);
            return;
        }

        pNewElement->m_pData = pValue;
        if(pElement->m_pNext)
            pNewElement->m_pNext = pElement->m_pNext;

        pElement->m_pNext = pNewElement;
    }
}

LPVOID HashMap_Get(PHASHMAP pHashMap, LPWSTR lpszKey) {
    SIZE_T hash;
    PHASHMAP_ELEMENT pElement;

    hash = HashMap_Hash(lpszKey);
    pElement = &(pHashMap->m_aElements[hash % pHashMap->m_cTableSize]);
    while(pElement && lstrcmpW(pElement->m_lpszKey, lpszKey) != 0) {
        pElement = pElement->m_pNext;
    }

    if(pElement) {
        return pElement->m_pData;
    }

    return NULL;
}