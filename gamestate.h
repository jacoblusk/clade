#ifndef GAMESTATE_H
#define GAMESTATE_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include "entity.h"

typedef struct tagGAMESTATE {
    PENTITY m_pCharacter;
} GAMESTATE, *PGAMESTATE;

void Update(PGAMESTATE pGameState);

#endif