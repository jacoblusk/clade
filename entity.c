#include "ioutil.h"
#include "entity.h"

PENTITY Entity_Create(void) {
    HANDLE hProcessHeap;
    PENTITY pEntity;

    hProcessHeap = GetProcessHeap();
    pEntity = HeapAlloc(hProcessHeap, HEAP_ZERO_MEMORY, sizeof(ENTITY));
    if(!pEntity) {
        return NULL;
    }

    pEntity->m_ySpeed = 1.0f;
    return pEntity;
}

BOOL Entity_Release(PENTITY *ppEntity) {
    BOOL bResult;
    HANDLE hProcessHeap;

    hProcessHeap = GetProcessHeap();
    bResult = HeapFree(hProcessHeap, 0, *ppEntity);
    if(!bResult) {
        Errorf(L"error: HeapFree failed to free PENTITY.\n");
    }

    *ppEntity = NULL;
    return bResult;
}