#include "graphics.h"
#include "error.h"

HRESULT
Render(PGRAPHICS pGraphics) {
    HRESULT lResult;

    lResult = CreateDeviceResources(pGraphics);
    if(SUCCEEDED(lResult)) {
        ID2D1Bitmap *pBitmap = NULL;
        D2D1_COLOR_F fColor = {0.0f, 255.0f, 255.0f, 255.0f};
        ID2D1HwndRenderTarget_BeginDraw(pGraphics->m_phRenderTarget);
        {
            ID2D1HwndRenderTarget_Clear(pGraphics->m_phRenderTarget, &fColor);
            lResult = LoadBitmapFromFileW(pGraphics, L"test.png", &pBitmap);
            if(SUCCEEDED(lResult)) {
                ID2D1HwndRenderTarget_DrawBitmap(
                    pGraphics->m_phRenderTarget,
                    pBitmap,
                    NULL,
                    1.0f,
                    D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
                    NULL);
            }
        }

        ID2D1HwndRenderTarget_EndDraw(pGraphics->m_phRenderTarget, NULL, NULL);
        ID2D1Bitmap_Release(pBitmap);
    }

    return lResult;
}

HRESULT
LoadBitmapFromFileW(PGRAPHICS pGraphics, 
    LPWSTR lpszFilename,
    ID2D1Bitmap **ppBitmap) {

    HRESULT lResult = S_OK;
    IWICBitmapDecoder *pDecoder = NULL;
    IWICBitmapFrameDecode *pSource = NULL;
    IWICFormatConverter *pConverter = NULL;

    lResult = IWICImagingFactory_CreateDecoderFromFilename(
        pGraphics->m_pWICImagingFactory,
        lpszFilename,
        (const GUID *) NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnDemand,
        &pDecoder
    );

    if(FAILED(lResult)) {
        WriteError(L"error: failed to create IWICBitmapDecoder.\n");
        return lResult;
    } 

    lResult = IWICBitmapDecoder_GetFrame(pDecoder, 0, &pSource);
    if(FAILED(lResult)) {
        WriteError(L"error: IWICBitmapDecoder_GetFrame failed: %d\n", lResult);
        return lResult;
    }

    lResult = IWICImagingFactory_CreateFormatConverter(pGraphics->m_pWICImagingFactory, &pConverter);
    if(FAILED(lResult)) {
        WriteError(L"error: IWICImagingFactory_CreateFormatConverter failed: %d\n", lResult);
        return lResult;
    }

    lResult = IWICFormatConverter_Initialize_Proxy(
        pConverter,
        (IWICBitmapSource *)pSource,
        &GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        NULL,
        0.0f,
        WICBitmapPaletteTypeMedianCut
    );

    if(FAILED(lResult)) {
        WriteError(L"error: IWICFormatConverter_Initialize failed: %d\n", lResult);
        return lResult;
    }

    lResult = ID2D1HwndRenderTarget_CreateBitmapFromWicBitmap(
        pGraphics->m_phRenderTarget,
        (IWICBitmapSource *)pConverter,
        NULL,
        ppBitmap
    );

    if(FAILED(lResult)) {
        WriteError(L"error: ID2D1HwndRenderTarget_CreateBitmapFromWicBitmap failed: %d\n", lResult);
        return lResult;
    }

    IWICBitmapDecoder_Release(pDecoder);
    IWICBitmapFrameDecode_Release(pSource);    
    IWICFormatConverter_Release(pConverter);

    return lResult;
}

HRESULT CreateDeviceResources(PGRAPHICS pGraphics) {
    HRESULT lResult;

    lResult = S_OK;
    if(!pGraphics->m_phRenderTarget) {
        RECT rcClientRect;
        D2D1_SIZE_U uSize;
        D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;
        D2D1_HWND_RENDER_TARGET_PROPERTIES hWndRenderTargetProperties;
        const D2D1_PIXEL_FORMAT pixelFormat = { DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_UNKNOWN };

        GetClientRect(pGraphics->m_hWnd, &rcClientRect);
        uSize.width = rcClientRect.right - rcClientRect.left;
        uSize.height = rcClientRect.bottom - rcClientRect.top;

        renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
        renderTargetProperties.pixelFormat = pixelFormat;
        renderTargetProperties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
        renderTargetProperties.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
        renderTargetProperties.dpiX = renderTargetProperties.dpiY = 0.0f;

        hWndRenderTargetProperties.hwnd = pGraphics->m_hWnd;
        hWndRenderTargetProperties.pixelSize = uSize;
        hWndRenderTargetProperties.presentOptions = D2D1_PRESENT_OPTIONS_NONE;

        if(FAILED(lResult = ID2D1Factory_CreateHwndRenderTarget(
            pGraphics->m_pDirect2DFactory,
            &renderTargetProperties,
            &hWndRenderTargetProperties,
            &pGraphics->m_phRenderTarget))) {
                WriteError(L"error: ID2D1Factory_CreateHwndRenderTarget\n");
                return lResult;
        }
    }

    return lResult;
}

void ReleaseDeviceResources(PGRAPHICS pGraphics) {
    ID2D1HwndRenderTarget_Release(pGraphics->m_phRenderTarget);
}

PGRAPHICS
CreateDeviceIndependentResources(void) {
    HANDLE hProcessHeap;
    HRESULT lResult;
    PGRAPHICS pGraphics;
    const D2D1_FACTORY_OPTIONS opts = { D2D1_DEBUG_LEVEL_INFORMATION };

    hProcessHeap = GetProcessHeap();
    pGraphics = HeapAlloc(hProcessHeap, HEAP_ZERO_MEMORY, sizeof(GRAPHICS));
    if(!pGraphics) {
        WriteError(L"error: HeapAlloc failed to allocate PGRAPHICS.\n");
        return NULL;
    }

    pGraphics->m_pDirect2DFactory = NULL;
    lResult = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        (const IID *)&IID_ID2D1Factory,
        &opts,
        (void **)&pGraphics->m_pDirect2DFactory
    );

    if(FAILED(lResult)) {
        BOOL bResult;

        WriteError(L"error: D2D1CreateFactory failed.\n");
        bResult = HeapFree(hProcessHeap, 0, pGraphics);
        if(!bResult) {
            WriteError(L"error: HeapFree failed to free PGRAPHICS.\n");
        }

        return NULL;
    }

    CoInitialize(NULL);

    lResult = CoCreateInstance(
        &CLSID_WICImagingFactory,
        (LPUNKNOWN) NULL,
        CLSCTX_INPROC_SERVER,
        &IID_IWICImagingFactory,
        (void **)&pGraphics->m_pWICImagingFactory
    );

    if(FAILED(lResult)) {
        BOOL bResult;

        WriteError(L"error: (%d) CoCreateInstance failed to create IWICImagingFactory.\n", lResult);
        bResult = HeapFree(hProcessHeap, 0, pGraphics);
        if(!bResult) {
            WriteError(L"error: HeapFree failed to free PGRAPHICS.\n");
        }

        return NULL;
    }

    return pGraphics;
}

void
ReleaseDeviceIndependentResources(PGRAPHICS *ppGraphics) {
    HANDLE hProcessHeap;
    BOOL bResult;

    ID2D1Factory_Release((*ppGraphics)->m_pDirect2DFactory);
    hProcessHeap = GetProcessHeap();
    bResult = HeapFree(hProcessHeap, 0, *ppGraphics);
    if(!bResult) {
        WriteError(L"error: HeapFree failed to free PGRAPHICS.\n");
    }

    *ppGraphics = NULL;
}