#include "graphics.h"
#include "ioutil.h"
#include "gamestate.h"
#include "entity.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void WINAPI
start(void) {
    HWND hWnd;
    WNDCLASSW wc = {0};
    MSG uMsg;
    BOOL bRunning;
    PGRAPHICS pGraphics;
    GAMESTATE gameState;

    bRunning = TRUE;
    Errorf(L"Hello world! %d\n", 1);

    gameState.m_pCharacter = Entity_Create();
    if(!gameState.m_pCharacter) {
        Errorf(L"error: failed to CreateEntity.");
        ExitProcess(EXIT_FAILURE);
        return;
    }

    pGraphics = CreateDeviceIndependentResources();
    if(!pGraphics) {
        Errorf(L"error: CreateDeviceIndependentResources failed.\n");
        ExitProcess(EXIT_FAILURE);
        return;
    }

    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = (HINSTANCE) GetModuleHandle(NULL);
    wc.cbWndExtra = sizeof(GRAPHICS);
    wc.lpszClassName = L"Window Class";

    if(!RegisterClassW(&wc)) {
        Errorf(L"error: RegisterClassW failed.\n");
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
        Errorf(L"error: CreateWindowExW failed.\n");
        ExitProcess(EXIT_FAILURE);
        return;
    }

    SetLastError(0);
    if(!SetWindowLongPtrW(hWnd, 0, (LONG_PTR)pGraphics) &&
        GetLastError() != 0) {
        Errorf(L"error: SetWindowLongPtrW failed.\n");
    }

    pGraphics->m_hWnd = hWnd;
    if(FAILED(CreateDeviceResources(pGraphics))) {
        Errorf(L"error: CreateDeviceResources failed.\n");
        return;
    }

    while(bRunning) {
        BOOL bResult;
        while((bResult = PeekMessage(&uMsg, NULL, 0, 0, PM_REMOVE)) != 0) {
            if (bResult == -1 ||
                uMsg.message == WM_QUIT) {
                bRunning = FALSE;
            }

            TranslateMessage(&uMsg);
            DispatchMessage(&uMsg);
        }

        if(FAILED(Render(pGraphics, &gameState))) {
            Errorf(L"error: Render failed.\n");
            return;
        }
    }

    Entity_Release(&gameState.m_pCharacter);
    ReleaseDeviceResources(pGraphics);
    ReleaseDeviceIndependentResources(&pGraphics);
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