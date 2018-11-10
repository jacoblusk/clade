#include "graphics.h"
#include "ioutil.h"
#include "gamestate.h"
#include "entity.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

VOID WINAPI
start(VOID) {
    HWND hWnd;
    WNDCLASSW wc = {0};
    MSG uMsg;
    BOOL bRunning;
    PGRAPHICS pGraphics;
    GAMESTATE gameState;

    bRunning = TRUE;
    GameTimer_Initialize(&gameState.m_GameTimer);
    gameState.m_pCharacter = Entity_Create();
    if(!gameState.m_pCharacter) {
        ErrorfW(L"error: failed to CreateEntity.");
        ExitProcess(EXIT_FAILURE);
        return;
    }

    pGraphics = Graphics_CreateDeviceIndependentResources();
    if(!pGraphics) {
        ErrorfW(L"error: CreateDeviceIndependentResources failed.\n");
        ExitProcess(EXIT_FAILURE);
        return;
    }

    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = (HINSTANCE) GetModuleHandle(NULL);
    wc.cbWndExtra = sizeof(GRAPHICS);
    wc.lpszClassName = L"Window Class";

    if(!RegisterClassW(&wc)) {
        ErrorfW(L"error: RegisterClassW failed.\n");
        ExitProcess(EXIT_FAILURE);
        return;
    }

    hWnd = CreateWindowExW(
        (DWORD) 0,
        wc.lpszClassName,
        L"Window Name!",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (HWND) NULL,
        (HMENU) NULL,
        (HINSTANCE) GetModuleHandle(NULL),
        (LPVOID) NULL
    );

    if(!hWnd) {
        ErrorfW(L"error: CreateWindowExW failed.\n");
        ExitProcess(EXIT_FAILURE);
        return;
    }

    SetLastError(0);
    if(!SetWindowLongPtrW(hWnd, 0, (LONG_PTR)pGraphics) &&
        GetLastError() != 0) {
        ErrorfW(L"error: SetWindowLongPtrW failed.\n");
    }

    pGraphics->m_hWnd = hWnd;
    if(FAILED(Graphics_CreateDeviceResources(pGraphics))) {
        ErrorfW(L"error: CreateDeviceResources failed.\n");
        return;
    }

    GameTimer_Start(&gameState.m_GameTimer);
    BOOL caKeyStates[255];
    for(BYTE i = 0; i < 255; i++) {
        caKeyStates[i] = 0;
    }

    while(bRunning) {
        BOOL bResult;

        while((bResult = PeekMessage(&uMsg, NULL, 0, 0, PM_REMOVE)) != 0) {
            if (bResult == -1 ||
                uMsg.message == WM_QUIT) {
                PrintfW(L"Quit was pressed!\n");
                bRunning = FALSE;
                goto end;
            }

            TranslateMessage(&uMsg);
            DispatchMessage(&uMsg);
        }

        gameState.m_frameInput.m_cInputLength = 0;
        for(UCHAR i = 0; i < UCHAR_MAX; i++) {
            SHORT keyState = GetAsyncKeyState(i);
            BOOL bKeyIsDown = (keyState & 0x8000) != 0;
            BOOL bKeyWasDown = caKeyStates[i];

            if((bKeyIsDown || bKeyWasDown) && (gameState.m_frameInput.m_cInputLength < (FRAMEINPUT_MAX_INPUTS - 1))) {
                PKEY_INPUT pCurrentInput = &gameState.m_frameInput.m_inputs[gameState.m_frameInput.m_cInputLength++];
                pCurrentInput->m_vkCode = i;
                pCurrentInput->m_bKeyIsDown = bKeyIsDown;
                pCurrentInput->m_bKeyWasDown = bKeyWasDown;
            }

            caKeyStates[i] = keyState;
        }

        GameTimer_Stop(&gameState.m_GameTimer);
        if(FAILED(Graphics_Render(pGraphics, &gameState))) {
            ErrorfW(L"error: Render failed.\n");
            return;
        }
        GameTimer_Reset(&gameState.m_GameTimer);
    }

  end:
    HashMap_Release(&pGraphics->m_pBitmapHashMap);
    Entity_Release(&gameState.m_pCharacter);
    Graphics_ReleaseDeviceResources(pGraphics);
    Graphics_ReleaseDeviceIndependentResources(&pGraphics);
    ExitProcess(EXIT_SUCCESS);
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
        LPARAM lParam) {
    LRESULT lResult;

    lResult = 0;
    switch(uMsg) {
        case WM_SIZE: {
            PGRAPHICS pGraphics = (PGRAPHICS)GetWindowLongPtrW(hWnd, 0);
            if(pGraphics && pGraphics->m_phRenderTarget) {
                D2D1_SIZE_U uSize;

                uSize.width = LOWORD(lParam);
                uSize.height = HIWORD(lParam);
                ID2D1HwndRenderTarget_Resize(pGraphics->m_phRenderTarget, &uSize);
            }

            lResult = DefWindowProc(hWnd, uMsg, wParam, lParam);
        } break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default: {
            lResult = DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }

    return lResult;
}