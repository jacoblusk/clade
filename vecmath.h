#ifndef VECMATH_H
#define VECMATH_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include <windows.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.707106781186547524400844362104849039 /* 1/sqrt(2) */
#endif

typedef struct tagFVECTOR2 {
    union {
        struct {
            FLOAT x;
            FLOAT y;
        };
        FLOAT e[2];
    };
} FVECTOR2;

FLOAT FVector2_Mag(FVECTOR2 a);
FVECTOR2 FVector2_Clamp(FVECTOR2 a, FVECTOR2 lb, FVECTOR2 ub);
FVECTOR2 FVector2_Add(FVECTOR2 a, FVECTOR2 b);
FVECTOR2 FVector2_AddV(INT cLength, ...);
FVECTOR2 FVector2_Sub(FVECTOR2 a, FVECTOR2 b);
FVECTOR2 FVector2_MulScalar(FLOAT a, FVECTOR2 b);
FLOAT FVector2_Dot(FVECTOR2 a, FVECTOR2 b);

FLOAT FSquare(FLOAT a);

#endif
