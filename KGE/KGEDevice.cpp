#include "stdafx.h"
#include "KGEDevice.h"

#include "KGEMath.h"

namespace KGE
{
    KGEDevice::KGEDevice()
        :_isInited(false)
    {

    }

    KGEDevice::~KGEDevice()
    {

    }

    bool KGEDevice::init(const Vector4 & viewport)
    {
        if (_isInited)
            return false;

        _isInited = true;

        _viewport = viewport;

        return true;
    }

    void KGEDevice::ViewPort(Vector4 & viewport)
    {
        viewport = _viewport;
    }

    void KGEDevice::DrawPoint(HDC hdc, int x, int y, const Vector4 & color)
    {
        int r_clamp = max(0, min(1, (int)color.x)) * 255;
        int g_clamp = max(0, min(1, (int)color.y)) * 255;
        int b_clamp = max(0, min(1, (int)color.z)) * 255;

        ::SetPixel(hdc, x, y, RGB(r_clamp, g_clamp, b_clamp));
    }

    void KGEDevice::DrawLine(HDC hdc, int x, int y, int xEnd, int yEnd, const Vector4 & color)
    {
        int offsetX = xEnd - x;
        int offsetY = yEnd - y;

        float x0 = x;
        float y0 = y;

        int steps = max(abs(offsetX), abs(offsetY));

        float xIncrement = float(offsetX) / steps;
        float yIncrement = float(offsetY) / steps;

        DrawPoint(hdc, x, y, color);

        for (int i = 0; i < steps; ++i)
        {
            x0 += xIncrement;
            y0 += yIncrement;
            DrawPoint(hdc, lroundf(x0), lroundf(y0), color);
        }
    }

    void KGEDevice::Clear(HDC hdc, const Vector4 & viewport, DWORD color)
    {
        ::BitBlt(hdc, (int)viewport.x, (int)viewport.y, (int)viewport.z, (int)viewport.w, NULL, NULL, NULL, color);
    }

    void KGEDevice::Present(HDC hdc, HDC hdcSrc, const Vector4 & viewport)
    {
        ::BitBlt(hdc, (int)viewport.x, (int)viewport.y, (int)viewport.z, (int)viewport.w, hdcSrc, NULL, NULL, SRCCOPY);
    }

    void KGEDevice::SoftRasterization(HDC hdc)
    {
        DrawPoint(hdc, 100, 100, Vector4(1, 0, 0, 1));
        DrawLine(hdc, 200, 247, 300, 405, Vector4(1, 0, 0, 1));
    }

    void KGEDevice::SoftRasterization_solid(HDC hdc)
    {

    }

    void KGEDevice::SoftRasterization_edge(HDC hdc)
    {

    }

}; // end of namespace KGE
