#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include <windows.h>

typedef VOID (*PNODE_MAPPROC)(LPVOID pValue);
typedef BOOL (*CPNODE_CMPPROC)(LPVOID pValue1, LPVOID pValue2);

typedef struct tagNODE NODE, *PNODE;
struct tagNODE {
    LPVOID m_pValue;
    PNODE m_pNext;
};

typedef struct tagLINKEDLIST {
    PNODE m_pHead;
    SIZE_T m_cLength;
    CPNODE_CMPPROC m_pfnCmpProc;
} LINKEDLIST, *PLINKEDLIST;

PLINKEDLIST LinkedList_Create(CPNODE_CMPPROC pfnCmpProc);
void LinkedList_Release(PLINKEDLIST *ppLinkedList, PNODE_MAPPROC pfnFreeProc);
void LinkedList_Append(PLINKEDLIST pLinkedList, LPVOID pValue);
void LinkedList_Prepend(PLINKEDLIST pLinkedList, LPVOID pValue);
void LinkedList_Remove(PLINKEDLIST pLinkedList, LPVOID pValue);
void LinkedList_MapFn(PLINKEDLIST pLinkedList, PNODE_MAPPROC pfnMapProc);

#endif