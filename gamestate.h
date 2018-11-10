#ifndef GAMESTATE_H
#define GAMESTATE_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include "entity.h"

#define FRAMEINPUT_MAX_INPUTS 255

typedef struct tagGAMETIMER {
    LARGE_INTEGER m_frequency; 
    UINT          m_uMonitorRefreshHz;
    LARGE_INTEGER m_startCounter;
    LARGE_INTEGER m_endCounter;
} GAMETIMER, *PGAMETIMER;

typedef struct tagKEY_INPUT {
    UINT32 m_vkCode;
    BOOL   m_bKeyWasDown;
    BOOL   m_bKeyIsDown;
} KEY_INPUT, *PKEY_INPUT;

typedef struct tagFRAMEINPUT {
    KEY_INPUT m_inputs[FRAMEINPUT_MAX_INPUTS];

    UINT32 m_cInputLength;
    UINT32 m_xMousePosition;
    UINT32 m_yMousePosition;
} FRAMEINPUT;

typedef struct tagGAMESTATE {
    PENTITY    m_pCharacter;
    GAMETIMER  m_GameTimer;
    FRAMEINPUT m_frameInput;
} GAMESTATE, *PGAMESTATE;

VOID GameTimer_Initialize(PGAMETIMER pGameTimer);
VOID GameTimer_Start(PGAMETIMER pGameTimer);
VOID GameTimer_Stop(PGAMETIMER pGameTimer);
VOID GameTimer_Reset(PGAMETIMER pGameTimer);
FLOAT GameTimer_GetTimeDelta(PGAMETIMER pGameTimer);
FLOAT GameTimer_GetFPS(PGAMETIMER pGameTimer);

#endif