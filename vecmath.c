#include "vecmath.h"
#include "intrinsics.h"

FLOAT
FVector2_Mag(FVECTOR2 a) {
    return Sqrt(a.x * a.x + a.y + a.y);
}

FVECTOR2
FVector2_Clamp(FVECTOR2 a, FVECTOR2 lb, FVECTOR2 ub) {
    FVECTOR2 result;
    result.x = MAX(a.x, lb.x);
    result.y = MAX(a.y, lb.y);

    result.x = MIN(result.x, ub.x);
    result.y = MIN(result.y, ub.y);

    return result;
}

FLOAT
FSquare(FLOAT a) {
    return a * a;
}

FVECTOR2
FVector2_Add(FVECTOR2 a, FVECTOR2 b) {
    FVECTOR2 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

FVECTOR2
FVector2_AddV(INT cLength, ...) {
    va_list list;
    FVECTOR2 result = {.x = 0.0f, .y = 0.0f};

    va_start(list, cLength);
    for(INT i = 0; i < cLength; i++) {
        result = FVector2_Add(result, va_arg(list, FVECTOR2));
    }

    va_end(list);
    return result;
}

FVECTOR2
FVector2_Sub(FVECTOR2 a, FVECTOR2 b) {
    FVECTOR2 result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;

    return result;
}

FVECTOR2
FVector2_MulScalar(FLOAT a, FVECTOR2 b) {
    FVECTOR2 result;

    result.x = a * b.x;
    result.y = a * b.y;

    return result;
}

FLOAT
FVector2_Dot(FVECTOR2 a, FVECTOR2 b) {
    FLOAT result;

    result = (a.x * b.x) + (a.y * b.y);

    return result;
}
