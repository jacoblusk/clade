#include "error.h"
#include "entity.h"

PENTITY CreateEntity(void) {
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

BOOL ReleaseEntity(PENTITY *ppEntity) {
    BOOL bResult;
    HANDLE hProcessHeap;

    hProcessHeap = GetProcessHeap();
    bResult = HeapFree(hProcessHeap, 0, *ppEntity);
    if(!bResult) {
        WriteError(L"error: HeapFree failed to free PENTITY.\n");
    }

    *ppEntity = NULL;
    return bResult;
}