#include "ioutil.h"
#include "entity.h"

PENTITY
Entity_Create(VOID) {
    HANDLE hProcessHeap;
    PENTITY pEntity;

    hProcessHeap = GetProcessHeap();
    pEntity = HeapAlloc(hProcessHeap, HEAP_ZERO_MEMORY, sizeof(ENTITY));
    if(!pEntity) {
        return NULL;
    }

    pEntity->m_bUp = FALSE;
    pEntity->m_v2Vel.x = 0.0f;
    pEntity->m_v2Vel.y = 0.0f;

    pEntity->m_v2Pos.x = 300.0f;
    pEntity->m_v2Pos.y = 300.0f;
    return pEntity;
}

BOOL
Entity_Release(PENTITY *ppEntity) {
    BOOL bResult;
    HANDLE hProcessHeap;

    hProcessHeap = GetProcessHeap();
    bResult = HeapFree(hProcessHeap, 0, *ppEntity);
    if(!bResult) {
        ErrorfW(L"error: HeapFree failed to free PENTITY.\n");
    }

    *ppEntity = NULL;
    return bResult;
}