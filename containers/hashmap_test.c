#include "../ioutil.h"
#include "hashmap.h"

void start(void) {
    PHASHMAP pHashMap;
    INT16 value1 = 1;
    INT16 *outValue;

    pHashMap = HashMap_Create();
    HashMap_Put(pHashMap, L"test", &value1);
    HashMap_Get(pHashMap, L"test", (LPVOID *)&outValue);

    Printf(L"value is: %d\n", *outValue);
    HashMap_Release(&pHashMap);
}