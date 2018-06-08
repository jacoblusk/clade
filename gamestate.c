#include "gamestate.h"
#include "entity.h"
#include "ioutil.h"

VOID
GameState_Update(PGAMESTATE pGameState) {
    if(pGameState->m_frameInput.m_cInputLength > 0) {
        for(UINT32 i = 0; i < pGameState->m_frameInput.m_cInputLength; i++) {
            KEY_INPUT keyInput = pGameState->m_frameInput.m_inputs[i];
            if(keyInput.m_vkCode == 'A' && keyInput.m_bKeyIsDown) {
                pGameState->m_pCharacter->m_xPos -= 128.0f * GameTimer_GetTimeDelta(&pGameState->m_GameTimer);
            }

            if(keyInput.m_vkCode == 'D' && keyInput.m_bKeyIsDown) {
                pGameState->m_pCharacter->m_xPos += 128.0f * GameTimer_GetTimeDelta(&pGameState->m_GameTimer);
            }
        }
    }

    pGameState->m_pCharacter->m_yPos += 256 * GameTimer_GetTimeDelta(&pGameState->m_GameTimer) * pGameState->m_pCharacter->m_ySpeed;

    if(pGameState->m_pCharacter->m_yPos >= 600.0) {
        pGameState->m_pCharacter->m_yPos = 600.0;
        pGameState->m_pCharacter->m_ySpeed = -pGameState->m_pCharacter->m_ySpeed;
    }

    if(pGameState->m_pCharacter->m_yPos <= 0.0) {
        pGameState->m_pCharacter->m_ySpeed = -pGameState->m_pCharacter->m_ySpeed;;
    }
}

VOID
GameTimer_Initialize(PGAMETIMER pGameTimer) {
    QueryPerformanceFrequency(&pGameTimer->m_frequency);
}

VOID
GameTimer_Start(PGAMETIMER pGameTimer) {
    QueryPerformanceCounter(&pGameTimer->m_startCounter);
}

VOID
GameTimer_Stop(PGAMETIMER pGameTimer) {
    QueryPerformanceCounter(&pGameTimer->m_endCounter);
}

VOID
GameTimer_Reset(PGAMETIMER pGameTimer) {
    pGameTimer->m_startCounter = pGameTimer->m_endCounter;
}

FLOAT
GameTimer_GetTimeDelta(PGAMETIMER pGameTimer) {
    return ((FLOAT)(pGameTimer->m_endCounter.QuadPart - pGameTimer->m_startCounter.QuadPart)) / \
            (FLOAT)pGameTimer->m_frequency.QuadPart;
}

FLOAT
GameTimer_GetFPS(PGAMETIMER pGameTimer) {
    return pGameTimer->m_frequency.QuadPart / \
           ((FLOAT)(pGameTimer->m_endCounter.QuadPart - pGameTimer->m_startCounter.QuadPart));
}