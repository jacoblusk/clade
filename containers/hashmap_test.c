#include "../ioutil.h"
#include "hashmap.h"

void start(void) {
    PHASHMAP pHashMap;
    SIZE_T i;
    SIZE_T cFailCount;
    INT16 aIntArray[1000];

    cFailCount = 0;
    pHashMap = HashMap_Create();
    for(i = 0; i < 1000; i++) {
        WCHAR awcBuffer[1025];

        aIntArray[i] = (INT16)i;
        wsprintfW(awcBuffer, L"test %d", i);
        HashMap_Put(pHashMap, awcBuffer, &aIntArray[i]);
    }

    for(i = 0; i < 1000; i++) {
        INT16 *outValue;
        WCHAR awcBuffer[1025];

        wsprintfW(awcBuffer, L"test %d", i);
        outValue = HashMap_Get(pHashMap, awcBuffer);
        if(!outValue || *outValue != (INT16)i) {
            cFailCount++;
            Errorf(L"failed to get %d\n", i);
        } else {
            Errorf(L"got value: %d for (%d)\n", *outValue, i);
        }
    }

    Printf(L"Got %d out of %d\n", 1000 - cFailCount, 1000);

    ExitProcess(EXIT_SUCCESS);
    /* HashMap_Release(&pHashMap); */
}