#include "graphics.h"
#include "error.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void WINAPI
start(void) {
    HWND hWnd;
    WNDCLASSW wc = {0};
    MSG uMsg;
    BOOL bRunning;
    PGRAPHICS pGraphics;

    bRunning = TRUE;
    WriteError(L"Hello world! %d\n", 1);

    pGraphics = CreateDeviceIndependentResources();
    if(!pGraphics) {
        WriteError(L"error: CreateDeviceIndependentResources failed.\n");
        ExitProcess(EXIT_FAILURE);
        return;
    }

    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = (HINSTANCE) GetModuleHandle(NULL);
    wc.cbWndExtra = sizeof(GRAPHICS);
    wc.lpszClassName = L"Window Class";

    if(!RegisterClassW(&wc)) {
        WriteError(L"error: RegisterClassW failed.\n");
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
        WriteError(L"error: CreateWindowExW failed.\n");
        ExitProcess(EXIT_FAILURE);
        return;
    }

    SetLastError(0);
    if(!SetWindowLongPtrW(hWnd, 0, (LONG_PTR)pGraphics) &&
        GetLastError() != 0) {
        WriteError(L"error: SetWindowLongPtrW failed.\n");
    }

    pGraphics->m_hWnd = hWnd;
    if(FAILED(CreateDeviceResources(pGraphics))) {
        WriteError(L"error: CreateDeviceResources failed.\n");
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

        if(FAILED(Render(pGraphics))) {
            WriteError(L"error: Render failed.\n");
            return;
        }
    }

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