#include "intrinsics.h"

FLOAT Sqrt(FLOAT a) {
    __asm__( "fsqrt" : "+t" (a));
    return a;
}