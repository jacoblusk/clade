#include "linkedlist.h"

static BOOL __DefaultCmpProc(LPVOID pValue1, LPVOID pValue2) {
    return (pValue1 == pValue2);
}

PLINKEDLIST LinkedList_Create(CPNODE_CMPPROC pfnCmpProc) {
    HANDLE hProcessHeap;
    PLINKEDLIST pLinkedList;

    hProcessHeap = GetProcessHeap();
    pLinkedList = HeapAlloc(hProcessHeap, HEAP_ZERO_MEMORY, sizeof(LINKEDLIST));
    if(!pLinkedList) {
        return NULL;
    }

    if(!pfnCmpProc) {
        pLinkedList->m_pfnCmpProc = __DefaultCmpProc;
    }

    return pLinkedList;
}

void LinkedList_Prepend(PLINKEDLIST pLinkedList, LPVOID pValue) {
    HANDLE hProcessHeap;
    PNODE pNewNode;

    hProcessHeap = GetProcessHeap();
    pNewNode = HeapAlloc(hProcessHeap, 0, sizeof(NODE));
    if(!pNewNode)
        return;

    pNewNode->m_pNext = pLinkedList->m_pHead;
    pNewNode->m_pValue = pValue;

    pLinkedList->m_pHead = pNewNode;
}

void LinkedList_Release(PLINKEDLIST *ppLinkedList, PNODE_MAPPROC pfnFreeProc) {
    HANDLE hProcessHeap;
    PNODE pCurrentNode;
    PNODE pTempNode;

    if(!*ppLinkedList)
        return;

    hProcessHeap = GetProcessHeap();    
    pCurrentNode = (*ppLinkedList)->m_pHead;
    while(pCurrentNode) {
        pTempNode = pCurrentNode->m_pNext;
        if(pfnFreeProc)
            pfnFreeProc(pCurrentNode->m_pValue);
        HeapFree(hProcessHeap, 0, pCurrentNode);
        pCurrentNode = pTempNode;
    }

    HeapFree(hProcessHeap, 0, *ppLinkedList);
    *ppLinkedList = NULL;
}

void LinkedList_MapFn(PLINKEDLIST pLinkedList, PNODE_MAPPROC pfnMapProc) {
    PNODE pCurrentNode;

    pCurrentNode = pLinkedList->m_pHead;
    while(pCurrentNode) {
        pfnMapProc(pCurrentNode->m_pValue);
        pCurrentNode = pCurrentNode->m_pNext;
    }
}