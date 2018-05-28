#include "../ioutil.h"
#include "linkedlist.h"

void PrintIntValue(LPVOID pValue) {
    Printf(L"%d\n", *((INT16 *)pValue));
}

void start(void) {
    PLINKEDLIST pLinkedList;
    INT16 value1 = 1;
    INT16 value2 = 2;

    pLinkedList = LinkedList_Create(NULL);
    LinkedList_Prepend(pLinkedList, &value2);
    LinkedList_Prepend(pLinkedList, &value1);

    LinkedList_MapFn(pLinkedList, PrintIntValue);
    LinkedList_Release(&pLinkedList, NULL);

    ExitProcess(EXIT_SUCCESS);
}