#include "../ioutil.h"
#include "linkedlist.h"

void PrintIntValue(LPVOID pUserData, LPVOID pValue) {
    UNREFERENCED_PARAMETER(pUserData);

    Printf(L"%d\n", *((INT16 *)pValue));
}

BOOL Test_Length(void) {
    PLINKEDLIST pLinkedList;

    pLinkedList = LinkedList_Create((PNODE_CMPPROC) NULL);
    if(pLinkedList->m_cLength != 0) {
        Errorf(L"error: pLinkedList should be length 0 after creation, instead it's %d.\n", pLinkedList->m_cLength);
        return FALSE;
    }

    return TRUE;
}

BOOL Test_AppendRemoveAppend(void) {
    PLINKEDLIST pLinkedList;
    INT16 value1 = 1;
    INT16 value2 = 2;

    pLinkedList = LinkedList_Create((PNODE_CMPPROC) NULL);
    if(!pLinkedList)
        return FALSE;

    LinkedList_Append(pLinkedList, &value1);
    LinkedList_Remove(pLinkedList, &value1, FALSE);
    LinkedList_Append(pLinkedList, &value2);

    if(pLinkedList->m_cLength != 1) {
        Errorf(L"error: pLinkedList should be length 1, instead it's length %d.\n", pLinkedList->m_cLength);
        return FALSE;
    }

    if(*((INT16 *)pLinkedList->m_pHead->m_pValue) != value2) {
        Errorf(
            L"error: pLinkedList->m_pHead->m_Value should be 2, instead it's: %d.\n",
            *((INT16 *)pLinkedList->m_pHead->m_pValue)
        );
        return FALSE;
    }

    LinkedList_Release(&pLinkedList, (LPVOID) NULL, (PNODE_MAPPROC) NULL);
    return TRUE;
}

BOOL Test_PrependRemovePrepend(void) {
    PLINKEDLIST pLinkedList;
    INT16 value1 = 1;
    INT16 value2 = 2;

    pLinkedList = LinkedList_Create((PNODE_CMPPROC) NULL);
    if(!pLinkedList)
        return FALSE;

    LinkedList_Prepend(pLinkedList, &value1);
    LinkedList_Remove(pLinkedList, &value1, FALSE);
    LinkedList_Prepend(pLinkedList, &value2);

    if(pLinkedList->m_cLength != 1) {
        Errorf(L"error: pLinkedList should be length 1, instead it's length %d.\n", pLinkedList->m_cLength);
        return FALSE;
    }

    if(*((INT16 *)pLinkedList->m_pHead->m_pValue) != value2) {
        Errorf(
            L"error: pLinkedList->m_pHead->m_Value should be 2, instead it's: %d.\n",
            *((INT16 *)pLinkedList->m_pHead->m_pValue)
        );
        return FALSE;
    }

    LinkedList_Release(&pLinkedList, (LPVOID) NULL, (PNODE_MAPPROC) NULL);
    return TRUE;
}

void start(void) {
    if(!Test_AppendRemoveAppend()) {
        Errorf(L"failed: Test_AppendRemoveAppend.\n");
    } else {
        Errorf(L"passed: Test_AppendRemoveAppend\n");
    }

    if(!Test_PrependRemovePrepend()) {
        Errorf(L"failed: Test_PrependRemovePrepend.\n");
    } else {
        Errorf(L"passed: Test_PrependRemovePrepend\n");
    }

    if(!Test_Length()) {
        Errorf(L"failed: Test_Length.\n");
    } else {
        Errorf(L"passed: Test_Length\n");
    }

    ExitProcess(EXIT_SUCCESS);
}