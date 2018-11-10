#include "graphics.h"
#include "gamestate.h"
#include "entity.h"
#include "ioutil.h"

#define IGNORE_CAST_FUNCTION_TYPES(block) \
_Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Wcast-function-type\"") \
block \
_Pragma("GCC diagnostic pop")

typedef VOID (* fnID2D1HwndRenderTarget_GetPixelSize) (ID2D1RenderTarget *, D2D1_SIZE_U *);
VOID ID2D1HwndRenderTarget_GetPixelSizeFix(ID2D1HwndRenderTarget *this, D2D1_SIZE_U *puSize) {
    IGNORE_CAST_FUNCTION_TYPES({
        fnID2D1HwndRenderTarget_GetPixelSize fnGetPixelSize = (fnID2D1HwndRenderTarget_GetPixelSize) ((ID2D1RenderTarget *)this)->lpVtbl->GetPixelSize;
        (*fnGetPixelSize)((ID2D1RenderTarget *)this, puSize);
    })
}

VOID
Update(PGRAPHICS pGraphics, PGAMESTATE pGameState) {
    FVECTOR2 vDtPos = {.x = 0.0f, .y = 0.0f};
    D2D1_SIZE_U uSize;

    FLOAT dt = GameTimer_GetTimeDelta(&pGameState->m_GameTimer);
    ID2D1HwndRenderTarget_GetPixelSizeFix(pGraphics->m_phRenderTarget, &uSize);

    if(pGameState->m_frameInput.m_cInputLength > 0) {

        for(UINT32 i = 0; i < pGameState->m_frameInput.m_cInputLength; i++) {
            KEY_INPUT keyInput = pGameState->m_frameInput.m_inputs[i];

            if(keyInput.m_vkCode == 'A' && keyInput.m_bKeyIsDown) {
                vDtPos.x -= 8.0f;
            }

            if(keyInput.m_vkCode == 'D' && keyInput.m_bKeyIsDown) {
                vDtPos.x += 8.0f;
            }

            if(keyInput.m_vkCode == 'W' && keyInput.m_bKeyIsDown && !keyInput.m_bKeyWasDown) {
                vDtPos.y -= 4.0f / dt;
            }
        }
    }

    //Hacky

    vDtPos = FVector2_Add(vDtPos, FVector2_MulScalar(9.8f, (FVECTOR2){.x = 0.0f, .y = 1.0f}));
    vDtPos = FVector2_MulScalar(10.0f, vDtPos);

    //Overall.
    //vDtPos = FVector2_Add(vDtPos, FVector2_MulScalar(-0.2, pGameState->m_pCharacter->m_v2Vel));
    //Just the X.
    vDtPos = FVector2_Add(vDtPos, (FVECTOR2){.x = pGameState->m_pCharacter->m_v2Vel.x * -0.5f, .y = 0.0f});

    /*if(pGameState->m_pCharacter->m_v2Pos.y == uSize.height - 32 || pGameState->m_pCharacter->m_v2Pos.y == 32) {
        vDtPos = FVector2_Add(vDtPos, (FVECTOR2){.x = pGameState->m_pCharacter->m_v2Vel.x * -0.2f, .y = 0.0f});
    }*/

    vDtPos = FVector2_MulScalar(UNITS_TO_PIXELS, vDtPos);

    FVECTOR2 vNewPos = FVector2_AddV(3, FVector2_MulScalar(0.5f * FSquare(dt), vDtPos),
                                        FVector2_MulScalar(dt, pGameState->m_pCharacter->m_v2Vel),
                                        pGameState->m_pCharacter->m_v2Pos);

    pGameState->m_pCharacter->m_v2Vel = FVector2_Add(FVector2_MulScalar(dt, vDtPos), pGameState->m_pCharacter->m_v2Vel);

    if(vNewPos.x > uSize.width || vNewPos.y > uSize.height -64 || vNewPos.x < 0 || vNewPos.y < 0) {
        FVECTOR2 vR;
        if(vNewPos.x > uSize.width) {
            pGameState->m_pCharacter->m_v2Vel.x = 0;
            vR = (FVECTOR2){.x = 1, .y = 0};
        }

        if(vNewPos.y > uSize.height -64) {
            pGameState->m_pCharacter->m_v2Vel.y = 0;
            vR = (FVECTOR2){.x = 0, .y = 1};
        }

        if(vNewPos.x < 0) {
            pGameState->m_pCharacter->m_v2Vel.x = 0;
            vR = (FVECTOR2){.x = -1, .y = 0};
        }

        if(vNewPos.y < 0) {
            pGameState->m_pCharacter->m_v2Vel.y = 0;
            vR = (FVECTOR2){.x = 0, .y = -1};
        }

        UNREFERENCED_PARAMETER(vR);

        //Only slide if we're going a great velocity.
        /*
        pGameState->m_pCharacter->m_v2Vel = FVector2_Sub(pGameState->m_pCharacter->m_v2Vel,
            FVector2_MulScalar(2 * FVector2_Dot(FVector2_MulScalar(2, pGameState->m_pCharacter->m_v2Vel), vR), vR));*/
            

        //vNewPos = FVector2_AddV(2, FVector2_MulScalar(dt, pGameState->m_pCharacter->m_v2Vel), vNewPos);
        vNewPos = FVector2_Clamp(vNewPos, (FVECTOR2){.x = 0, .y = 0}, (FVECTOR2){.x = uSize.width, .y = uSize.height - 64});
    }

    pGameState->m_pCharacter->m_v2Pos = vNewPos;
}

HRESULT
Graphics_Render(PGRAPHICS pGraphics, PGAMESTATE pGameState) {
    HRESULT lResult;
    D2D1_COLOR_F fColor = {0.0f, 255.0f, 255.0f, 255.0f};

    lResult = S_OK;
    Graphics_CreateDeviceResources(pGraphics);

    Update(pGraphics, pGameState);
    ID2D1HwndRenderTarget_BeginDraw(pGraphics->m_phRenderTarget);
    {
        ID2D1Bitmap *pGrassBitmap = NULL;
        ID2D1Bitmap *pGrassFlowerBitmap = NULL;
        ID2D1Bitmap *pGroundBitmap = NULL;

        ID2D1HwndRenderTarget_Clear(pGraphics->m_phRenderTarget, &fColor);
        pGrassBitmap = HashMap_Get(pGraphics->m_pBitmapHashMap, L"grass32x32");
        pGrassFlowerBitmap = HashMap_Get(pGraphics->m_pBitmapHashMap, L"grass_flower32x32");
        pGroundBitmap = HashMap_Get(pGraphics->m_pBitmapHashMap, L"ground32x32");
        ID2D1Bitmap *pGuyIdleBitmap = HashMap_Get(pGraphics->m_pBitmapHashMap, L"guy_64x64");
        ID2D1Bitmap *pRabbitIdleBitmap = HashMap_Get(pGraphics->m_pBitmapHashMap, L"rabbit_test2_sheet");

        if(!pGrassBitmap || !pGrassFlowerBitmap || !pGroundBitmap || !pGuyIdleBitmap || !pRabbitIdleBitmap) {
            ErrorfW(L"HashMap_Get failed.\n");
            lResult = E_FAIL;
        }

        if(SUCCEEDED(lResult)) {
            D2D1_RECT_F sourceRectangle = { 0.0f, 0.0f, 32.0f, 32.0f };
            D2D1_SIZE_U uSize;

            ID2D1HwndRenderTarget_GetPixelSizeFix(pGraphics->m_phRenderTarget, &uSize);

            for(UINT x = 0; x < uSize.width; x += 64) {
                for(UINT y = uSize.height - 64; y < uSize.height; y += 64) {
                    ID2D1Bitmap *pBitmapToDraw = pGroundBitmap;
                    D2D1_RECT_F destinationRectangle = (D2D1_RECT_F){ (FLOAT)x, (FLOAT)y, (FLOAT)(x + 64), (FLOAT)(y + 64) };

                    ID2D1HwndRenderTarget_DrawBitmap(
                        pGraphics->m_phRenderTarget,
                        pBitmapToDraw,
                        &destinationRectangle,
                        1.0f,
                        D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
                        &sourceRectangle);
                }
            }
        }

        FVECTOR2 playerPos = pGameState->m_pCharacter->m_v2Pos;
        playerPos.y -= 20;
        D2D1_RECT_F sourceRectangle = { 0.0f, 0.0f, 32.0f, 32.0f };
        D2D1_RECT_F destinationRectangle = (D2D1_RECT_F){ playerPos.x - 32, playerPos.y - 32, playerPos.x + 32, playerPos.y + 32 };
        ID2D1HwndRenderTarget_DrawBitmap(
                        pGraphics->m_phRenderTarget,
                        pGuyIdleBitmap,
                        &destinationRectangle,
                        1.0f,
                        D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
                        &sourceRectangle);

        D2D1_SIZE_U uSize;
        ID2D1HwndRenderTarget_GetPixelSizeFix(pGraphics->m_phRenderTarget, &uSize);
        sourceRectangle = (D2D1_RECT_F){ 0.0f, 0.0f, 32.0f, 32.0f };

        uSize.height -= 64 + 16 - 6;
        destinationRectangle = (D2D1_RECT_F){ 800 - 16, uSize.height - 16, 800 + 16, uSize.height + 16 };
        ID2D1HwndRenderTarget_DrawBitmap(
                        pGraphics->m_phRenderTarget,
                        pRabbitIdleBitmap,
                        &destinationRectangle,
                        1.0f,
                        D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
                        &sourceRectangle);
        
        /*
        lResult = ID2D1HwndRenderTarget_CreateSolidColorBrush(
            pGraphics->m_phRenderTarget,
            &fEllipseColor,
            NULL,
            &pBrush
        );

        if(FAILED(lResult)) {
            ErrorfW(L"error: ID2D1HwndRenderTarget_CreateSolidColorBrush\n");
        } else {
            D2D1_POINT_2F point;

            point.x = pGameState->m_pCharacter->m_v2Pos.x;
            point.y = pGameState->m_pCharacter->m_v2Pos.y;
            ellipse.point = point;
            ellipse.radiusX = 30.0f;
            ellipse.radiusY = 30.0f;

            ID2D1HwndRenderTarget_DrawEllipse(
                pGraphics->m_phRenderTarget,
                &ellipse,
                (ID2D1Brush *) pBrush,
                1.0f,
                NULL
            );

            ID2D1HwndRenderTarget_FillEllipse(
                pGraphics->m_phRenderTarget,
                &ellipse,
                (ID2D1Brush *) pBrush
            );

            ID2D1SolidColorBrush_Release(pBrush);*/
    }

    HRESULT lResultEndDraw = ID2D1HwndRenderTarget_EndDraw(pGraphics->m_phRenderTarget, NULL, NULL);
    if(lResultEndDraw == (HRESULT)D2DERR_RECREATE_TARGET) {
        Graphics_ReleaseDeviceResources(pGraphics);
    }

    return lResult;
}

HRESULT
Graphics_LoadBitmapFromFileW(PGRAPHICS pGraphics, 
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
        ErrorfW(L"error: failed to create IWICBitmapDecoder.\n");
        return lResult;
    } 

    lResult = IWICBitmapDecoder_GetFrame(pDecoder, 0, &pSource);
    if(FAILED(lResult)) {
        ErrorfW(L"error: IWICBitmapDecoder_GetFrame failed: %d\n", lResult);
        return lResult;
    }

    lResult = IWICImagingFactory_CreateFormatConverter(pGraphics->m_pWICImagingFactory, &pConverter);
    if(FAILED(lResult)) {
        ErrorfW(L"error: IWICImagingFactory_CreateFormatConverter failed: %d\n", lResult);
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
        ErrorfW(L"error: IWICFormatConverter_Initialize failed: %d\n", lResult);
        return lResult;
    }

    lResult = ID2D1HwndRenderTarget_CreateBitmapFromWicBitmap(
        pGraphics->m_phRenderTarget,
        (IWICBitmapSource *)pConverter,
        NULL,
        ppBitmap
    );

    if(FAILED(lResult)) {
        ErrorfW(L"error: ID2D1HwndRenderTarget_CreateBitmapFromWicBitmap failed: %d\n", lResult);
        return lResult;
    }

    IWICBitmapDecoder_Release(pDecoder);
    IWICBitmapFrameDecode_Release(pSource);    
    IWICFormatConverter_Release(pConverter);

    return lResult;
}

static VOID FreeBitmapProc(LPVOID pUserData, LPVOID pValue) {
    UNREFERENCED_PARAMETER(pUserData);

    ID2D1Bitmap *pBitmap = pValue;
    if(pBitmap) {
        PrintfW(L"Freeing a bitmap!\n");
        ID2D1Bitmap_Release(pBitmap);
    }
}

HRESULT
Graphics_CreateDeviceResources(PGRAPHICS pGraphics) {
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
                ErrorfW(L"error: ID2D1Factory_CreateHwndRenderTarget\n");
                return lResult;
        }

        pGraphics->m_pBitmapHashMap = HashMap_Create((LPVOID) NULL, FreeBitmapProc);
        if(!pGraphics->m_pBitmapHashMap) {
            ErrorfW(L"error: HashMap_Create failed.\n");
            return E_FAIL;
        }

        ID2D1Bitmap *pGrassBitmap;
        ID2D1Bitmap *pGrassFlowerBitmap;
        ID2D1Bitmap *pGuyIdleBitmap;
        ID2D1Bitmap *pGroundBitmap;
        lResult = Graphics_LoadBitmapFromFileW(pGraphics, L"assets/grass32x32.png", &pGrassBitmap);
        if(SUCCEEDED(lResult)) {
            PrintfW(L"grass32x32 put in.\n");
            HashMap_Put(pGraphics->m_pBitmapHashMap, L"grass32x32", pGrassBitmap);
        }

        lResult = Graphics_LoadBitmapFromFileW(pGraphics, L"assets/grass_flower32x32.png", &pGrassFlowerBitmap);
        if(SUCCEEDED(lResult)) {
            PrintfW(L"grass_flower32x32 put in.\n");
            HashMap_Put(pGraphics->m_pBitmapHashMap, L"grass_flower32x32", pGrassFlowerBitmap);
        }

        lResult = Graphics_LoadBitmapFromFileW(pGraphics, L"assets/guy_front_idle64x32.png", &pGuyIdleBitmap);
        if(SUCCEEDED(lResult)) {
            PrintfW(L"guy_front_idle64x32 put in.\n");
            HashMap_Put(pGraphics->m_pBitmapHashMap, L"guy_64x64", pGuyIdleBitmap);
        }

        lResult = Graphics_LoadBitmapFromFileW(pGraphics, L"assets/ground32x32.png", &pGroundBitmap);
        if(SUCCEEDED(lResult)) {
            PrintfW(L"guy_front_idle64x32 put in.\n");
            HashMap_Put(pGraphics->m_pBitmapHashMap, L"ground32x32", pGroundBitmap);
        }

        ID2D1Bitmap *pRabbitBitmap;
        lResult = Graphics_LoadBitmapFromFileW(pGraphics, L"assets/rabbit_test2_sheet.png", &pRabbitBitmap);
        if(SUCCEEDED(lResult)) {
            PrintfW(L"rabbit_test2_sheet2 put in.\n");
            HashMap_Put(pGraphics->m_pBitmapHashMap, L"rabbit_test2_sheet", pRabbitBitmap);
        }
    }

    return lResult;
}

VOID
Graphics_ReleaseDeviceResources(PGRAPHICS pGraphics) {
    ID2D1HwndRenderTarget_Release(pGraphics->m_phRenderTarget);
}

PGRAPHICS
Graphics_CreateDeviceIndependentResources(VOID) {
    HANDLE hProcessHeap;
    HRESULT lResult;
    PGRAPHICS pGraphics;
    const D2D1_FACTORY_OPTIONS opts = { D2D1_DEBUG_LEVEL_INFORMATION };

    hProcessHeap = GetProcessHeap();
    pGraphics = HeapAlloc(hProcessHeap, HEAP_ZERO_MEMORY, sizeof(GRAPHICS));
    if(!pGraphics) {
        ErrorfW(L"error: HeapAlloc failed to allocate PGRAPHICS.\n");
        return NULL;
    }

    pGraphics->m_pDirect2DFactory = NULL;
    lResult = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        (const IID *)&IID_ID2D1Factory,
        &opts,
        (VOID **)&pGraphics->m_pDirect2DFactory
    );

    if(FAILED(lResult)) {
        ErrorfW(L"error: D2D1CreateFactory failed.\n");
        goto fail;
    }

    CoInitialize(NULL);

    lResult = CoCreateInstance(
        &CLSID_WICImagingFactory,
        (LPUNKNOWN) NULL,
        CLSCTX_INPROC_SERVER,
        &IID_IWICImagingFactory,
        (VOID **)&pGraphics->m_pWICImagingFactory
    );

    if(FAILED(lResult)) {
        ErrorfW(L"error: (%d) CoCreateInstance failed to create IWICImagingFactory.\n", lResult);
        goto fail;
    }

    return pGraphics;

  fail:
    HeapFree(hProcessHeap, 0, pGraphics);
    return NULL;
}

VOID
Graphics_ReleaseDeviceIndependentResources(PGRAPHICS *ppGraphics) {
    HANDLE hProcessHeap;

    ID2D1Factory_Release((*ppGraphics)->m_pDirect2DFactory);
    hProcessHeap = GetProcessHeap();
    if(*ppGraphics) {
        HeapFree(hProcessHeap, 0, *ppGraphics);
        *ppGraphics = NULL;
    }
}