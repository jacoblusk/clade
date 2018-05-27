#ifndef ENTITY_H
#define ENTITY_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include <windows.h>

typedef struct tagENTITY {
    int m_xPos;
    int m_yPos;
    float m_ySpeed; 
} ENTITY, *PENTITY;

PENTITY CreateEntity(void);
BOOL ReleaseEntity(PENTITY *ppEntity);

#endif