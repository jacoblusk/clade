#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include <windows.h>

typedef VOID (*PNODE_MAPPROC)(LPVOID pUserData, LPVOID pValue);
typedef BOOL (*PNODE_CMPPROC)(LPVOID pValue1, LPVOID pValue2);
typedef PNODE_MAPPROC PNODE_FREEPROC;

typedef struct tagNODE NODE, *PNODE;
struct tagNODE {
    LPVOID m_pValue;
    PNODE  m_pNext;
};

typedef struct tagLINKEDLIST {
    PNODE         m_pHead;
    SIZE_T        m_cLength;
    PNODE_CMPPROC m_pfnCmpProc;
} LINKEDLIST, *PLINKEDLIST;

PLINKEDLIST LinkedList_Create(PNODE_CMPPROC pfnCmpProc);
VOID        LinkedList_Release(PLINKEDLIST *ppLinkedList, LPVOID pUserData, PNODE_FREEPROC pfnFreeProc);
VOID        LinkedList_Append(PLINKEDLIST pLinkedList, LPVOID pValue);
VOID        LinkedList_Prepend(PLINKEDLIST pLinkedList, LPVOID pValue);
VOID        LinkedList_Remove(PLINKEDLIST pLinkedList, LPVOID pValue, BOOL bRemoveAll);
VOID        LinkedList_MapFn(PLINKEDLIST pLinkedList, LPVOID pUserData, PNODE_MAPPROC pfnMapProc);

#endif