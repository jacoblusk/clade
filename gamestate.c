#include "gamestate.h"
#include "entity.h"
#include "ioutil.h"
#include "vecmath.h"
#include "graphics.h"

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