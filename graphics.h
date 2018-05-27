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

PGRAPHICS CreateDeviceIndependentResources(void);
void ReleaseDeviceIndependentResources(PGRAPHICS *ppGraphics);
HRESULT CreateDeviceResources(PGRAPHICS pGraphics);
void ReleaseDeviceResources(PGRAPHICS pGraphics);
HRESULT Render(PGRAPHICS pGraphics, PGAMESTATE pGameState);
HRESULT LoadBitmapFromFileW(PGRAPHICS pGraphics, LPWSTR lpszFilename, ID2D1Bitmap **ppBitmap);

#endif