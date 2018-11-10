#ifndef ENTITY_H
#define ENTITY_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include <windows.h>
#include "vecmath.h"

typedef struct tagENTITY {
    FVECTOR2 m_v2Pos;
    FVECTOR2 m_v2Vel;
    BOOL     m_bUp;
} ENTITY, *PENTITY;

PENTITY Entity_Create(VOID);
BOOL    Entity_Release(PENTITY *ppEntity);

#endif