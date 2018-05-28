#include "linkedlist.h"

static BOOL __DefaultCmpProc(LPVOID pValue1, LPVOID pValue2) {
    return (pValue1 == pValue2);
}

PLINKEDLIST LinkedList_Create(PNODE_CMPPROC pfnCmpProc) {
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

    ++pLinkedList->m_cLength;
    pNewNode->m_pNext = pLinkedList->m_pHead;
    pNewNode->m_pValue = pValue;

    pLinkedList->m_pHead = pNewNode;
}

void LinkedList_Append(PLINKEDLIST pLinkedList, LPVOID pValue) {
    HANDLE hProcessHeap;
    PNODE pNewNode;
    PNODE pCurrentNode;

    hProcessHeap = GetProcessHeap();
    pNewNode = HeapAlloc(hProcessHeap, 0, sizeof(NODE));
    if(!pNewNode)
        return;

    pNewNode->m_pNext = NULL;
    pNewNode->m_pValue = pValue;

    pCurrentNode = pLinkedList->m_pHead;
    if(!pCurrentNode) {
        ++pLinkedList->m_cLength;
        pLinkedList->m_pHead = pNewNode;
        return;
    }

    while(pCurrentNode) {
        if(pCurrentNode->m_pNext && !pCurrentNode->m_pNext->m_pNext) {
            ++pLinkedList->m_cLength;
            pCurrentNode->m_pNext->m_pNext = pNewNode;
            break;
        } else if(!pCurrentNode->m_pNext) {
            ++pLinkedList->m_cLength;
            pCurrentNode->m_pNext = pNewNode;
            break;
        }

        pCurrentNode = pCurrentNode->m_pNext;
    }
}

void LinkedList_Remove(PLINKEDLIST pLinkedList, LPVOID pValue, BOOL bRemoveAll) {
    HANDLE hProcessHeap;
    PNODE pCurrentNode;
    PNODE pPreviousNode;

    hProcessHeap = GetProcessHeap();
    pPreviousNode = NULL;
    pCurrentNode = pLinkedList->m_pHead;
    while(pCurrentNode) {
        if(pLinkedList->m_pfnCmpProc(pCurrentNode->m_pValue, pValue)) {
            PNODE pTempNode;

            pTempNode = pCurrentNode->m_pNext;
            if(pPreviousNode) {
                pPreviousNode->m_pNext = pCurrentNode->m_pNext;
            } else {
                pLinkedList->m_pHead = pCurrentNode->m_pNext;
            }

            --pLinkedList->m_cLength;
            HeapFree(hProcessHeap, 0, pCurrentNode);
            if(bRemoveAll) {
                pCurrentNode = pTempNode;
                continue;
            } else {
                return;
            }
        }

        pPreviousNode = pCurrentNode;
        pCurrentNode = pCurrentNode->m_pNext;
    }
}

void LinkedList_Release(PLINKEDLIST *ppLinkedList, LPVOID pUserData, PNODE_MAPPROC pfnFreeProc) {
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
            pfnFreeProc(pUserData, pCurrentNode->m_pValue);
        HeapFree(hProcessHeap, 0, pCurrentNode);
        pCurrentNode = pTempNode;
    }

    HeapFree(hProcessHeap, 0, *ppLinkedList);
    *ppLinkedList = NULL;
}

void LinkedList_MapFn(PLINKEDLIST pLinkedList, LPVOID pUserData, PNODE_MAPPROC pfnMapProc) {
    PNODE pCurrentNode;

    pCurrentNode = pLinkedList->m_pHead;
    while(pCurrentNode) {
        pfnMapProc(pUserData, pCurrentNode->m_pValue);
        pCurrentNode = pCurrentNode->m_pNext;
    }
}