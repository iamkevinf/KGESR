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

    void KGEDevice::_DrawTriangle_loafer(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3, const Vector4 & color)
    {
        for (int y = y1; y <= y2; ++y)
        {
            int xs = (y - y1) * (x2 - x1) / (y2 - y1) + x1 + 0.5f;
            int xe = (y - y1) * (x3 - x1) / (y3 - y1) + x1 + 0.5f;

            DrawLine(hdc, xs, y, xe, y, color);
        }
    }

    void KGEDevice::_DrawTriangle_flat(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3, const Vector4 & color)
    {
        for (int y = y1; y <= y3; ++y)
        {
            int xs = (y - y1) * (x3 - x1) / (y3 - y1) + x1 + 0.5f;
            int xe = (y - y2) * (x3 - x2) / (y3 - y2) + x2 + 0.5f;

            DrawLine(hdc, xs, y, xe, y, color);
        }
    }

    void KGEDevice::DrawTriangle(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3, const Vector4 & color)
    {
        if (y1 == y2)
        {
            if (y3 <= y1)
                _DrawTriangle_loafer(hdc, x3, y3, x1, y1, x2, y2, color);
            else
                _DrawTriangle_flat(hdc, x1, y1, x2, y2, x3, y3, color);
        }
        else if (y1 == y3)
        {
            if (y2 <= y1)
                _DrawTriangle_loafer(hdc, x2, y2, x1, y1, x3, y3, color);
            else
                _DrawTriangle_flat(hdc, x1, y1, x2, y2, x3, y3, color);
        }
        else if (y2 == y3)
        {
            if (y1 <= y2)
                _DrawTriangle_loafer(hdc, x1, y1, x2, y2, x3, y3, color);
            else
                _DrawTriangle_flat(hdc, x1, y1, x2, y2, x3, y3, color);
        }
        else
        {
            double xtop, ytop, xmiddle, ymiddle, xbottom, ybottom;
            if (y1 < y2 && y2 < y3)
            {
                xtop = x1; ytop = y1;
                xmiddle = x2; ymiddle = y2;
                xbottom = x3; ybottom = y3;
            }
            else if (y1 < y3 && y3 < y2)
            {
                xtop = x1; ytop = y1;
                xmiddle = x3; ymiddle = y3;
                xbottom = x2; ybottom = y2;
            }
            else if (y2 < y1 && y1 < y3)
            {
                xtop = x2; ytop = y2;
                xmiddle = x1; ymiddle = y1;
                xbottom = x3; ybottom = y3;
            }
            else if (y2 < y3 && y3 < y1)
            {
                xtop = x2; ytop = y2;
                xmiddle = x3; ymiddle = y3;
                xbottom = x1; ybottom = y1;
            }
            else if (y3 < y1 && y1 < y2)
            {
                xtop = x3; ytop = y3;
                xmiddle = x1; ymiddle = y1;
                xbottom = x2; ybottom = x3;
            }
            else if (y3 < y2 && y2 < y1)
            {
                xtop = x3; ytop = y3;
                xmiddle = x2; ymiddle = y2;
                xbottom = x1; ybottom = x1;
            }

            int xl = (ymiddle - ytop) * (xbottom - xtop) / (ybottom - ytop) + xtop + 0.5f;

            if (xl <= xmiddle)
            {
                _DrawTriangle_loafer(hdc, xtop, ytop, xl, ymiddle, xmiddle, ymiddle, color);
                _DrawTriangle_flat(hdc, xl, ymiddle, xmiddle, ymiddle, xbottom, ybottom, color);
            }
            else
            {
                _DrawTriangle_loafer(hdc, xtop, ytop, xmiddle, ymiddle, xl, ymiddle, color);
                _DrawTriangle_flat(hdc, xmiddle, ymiddle, xl, ymiddle, xbottom, ybottom, color);
            }
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
        DrawTriangle(hdc, 500, 500, 400, 600, 700, 700, Vector4(1, 0, 0, 1));
    }

    void KGEDevice::SoftRasterization_solid(HDC hdc)
    {

    }

    void KGEDevice::SoftRasterization_edge(HDC hdc)
    {

    }

}; // end of namespace KGE
