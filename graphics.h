#ifndef GRAPHICS_H
#define GRAPHICS_H

#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE
#define COBJMACROS

#include <windows.h>
#include <d2d1.h>
#include <wincodec.h>

#include "gamestate.h"

typedef struct tagGRAPHICS {
    ID2D1Factory *m_pDirect2DFactory;
    ID2D1HwndRenderTarget *m_phRenderTarget;
    IWICImagingFactory *m_pWICImagingFactory;
    HWND m_hWnd;
} GRAPHICS, *PGRAPHICS;

PGRAPHICS Graphics_CreateDeviceIndependentResources(VOID);
VOID      Graphics_ReleaseDeviceIndependentResources(PGRAPHICS *ppGraphics);
HRESULT   Graphics_CreateDeviceResources(PGRAPHICS pGraphics);
VOID      Graphics_ReleaseDeviceResources(PGRAPHICS pGraphics);
HRESULT   Graphics_Render(PGRAPHICS pGraphics, PGAMESTATE pGameState);
HRESULT   Graphics_LoadBitmapFromFileW(PGRAPHICS pGraphics, LPWSTR lpszFilename, ID2D1Bitmap **ppBitmap);

#endif