#ifndef ENTITY_H
#define ENTITY_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include <windows.h>

typedef struct tagENTITY {
    FLOAT m_xPos;
    FLOAT m_yPos;
    FLOAT m_ySpeed;
    BOOL  m_bUp;
} ENTITY, *PENTITY;

PENTITY Entity_Create(VOID);
BOOL Entity_Release(PENTITY *ppEntity);

#endif