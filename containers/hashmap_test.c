#include "../ioutil.h"
#include "hashmap.h"

void start(void) {
    PHASHMAP pHashMap;
    SIZE_T i;
    INT16 *outValue;
    INT16 aIntArray[1000];

    pHashMap = HashMap_Create(NULL, NULL);
    for(i = 0; i < 1000; i++) {

        aIntArray[i] = (INT16)i;
        HashMap_Put(pHashMap, L"test 1", &aIntArray[i]);
    }

    outValue = HashMap_Get(pHashMap, L"test 1");
    Printf(L"outValue: %d\n", *outValue);

    HashMap_Release(&pHashMap);
    ExitProcess(EXIT_SUCCESS);
}