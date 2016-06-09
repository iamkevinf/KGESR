#include "stdafx.h"
#include "KGEDevice.h"

#include "KGEMath.h"
#include "KGEMesh.h"
#include "KGEVertex.h"

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

        _camera = new KGECamera(viewport.z, viewport.w);
        _camera->init(Vector4(1, 0, 2, 1), Vector4(0, 0, 0, 1), Vector4(0, 1, 0, 0));

        _zBuffer = new double[(int)viewport.z * (int)viewport.w];
        ZBufferClear();

        _mesh = new KGEMesh();
        _mesh->LoadFromFile("res/ASEModels/teaport.ASE");

        return true;
    }

    void KGEDevice::ViewPort(Vector4 & viewport)
    {
        viewport = _viewport;
    }

    ////////////////////////////////////////////////////////
    /// draw Primitive
    ////////////////////////////////////////////////////////

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
            if (y2 == y1 || y3 == y1)
                continue;

            int xs = (y - y1) * (x2 - x1) / (y2 - y1) + x1 + 0.5f;
            int xe = (y - y1) * (x3 - x1) / (y3 - y1) + x1 + 0.5f;

            DrawLine(hdc, xs, y, xe, y, color);
        }
    }

    void KGEDevice::_DrawTriangle_flat(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3, const Vector4 & color)
    {
        for (int y = y1; y <= y3; ++y)
        {
            if (x3 == x1 || y3 == y1)
                continue;

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
                xbottom = x2; ybottom = y2;
            }
            else if (y3 < y2 && y2 < y1)
            {
                xtop = x3; ytop = y3;
                xmiddle = x2; ymiddle = y2;
                xbottom = x1; ybottom = y1;
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

    ////////////////////////////////////////////////////////
    /// zBuffer
    ////////////////////////////////////////////////////////

    void KGEDevice::ZBufferClear()
    {
        const float clearValue = 1;
        int zBufferSize = (int)_viewport.z * (int)_viewport.w;

        for (int i = 0; i < zBufferSize; ++i)
        {
            _zBuffer[i] = clearValue;
        }
    }

    double KGEDevice::ZBufferRead(int x, int y)
    {
        const int w = (int)_viewport.z;
        const int h = (int)_viewport.w;

        if (x >= 0 && x < w && y >= 0 && y < h)
            return _zBuffer[y * w + x];
        else
            return 1;
    }

    void KGEDevice::ZBufferWrite(int x, int y, double value)
    {
        const int w = (int)_viewport.z;
        const int h = (int)_viewport.w;

        if (x >= 0 && x < w && y >= 0 && y < h)
            _zBuffer[y * w + x] = value;
    }

    ////////////////////////////////////////////////////////
    /// Rasterization
    ////////////////////////////////////////////////////////

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
        /// TODO
        /// for test hard code
        DrawPoint(hdc, 100, 100, Vector4(1, 0, 0, 1));
        DrawLine(hdc, 200, 247, 300, 405, Vector4(1, 0, 0, 1));
        DrawTriangle(hdc, 500, 200, 400, 300, 700, 400, Vector4(0, 1, 0, 1));

        int nIDtri = (int)_mesh->triList.size();
        for (int i = 0; i<nIDtri; i++){
            const KGETriangle & IDtri = _mesh->triList[i];
            const int vID0 = IDtri.vID(0);
            const int vID1 = IDtri.vID(1);
            const int vID2 = IDtri.vID(2);
            if (vID0 == -1)continue;
            KGEVertex v0, v1, v2;
            v0 = _mesh->GetVertex(vID0);
            v1 = _mesh->GetVertex(vID1);
            v2 = _mesh->GetVertex(vID2);

            DrawTriangle(hdc, 100 + v0.pos.x, 100 + v0.pos.y, 100 + v1.pos.x, 100 + v1.pos.y, 100 + v2.pos.x, 100 + v2.pos.y, v0.color);
        }
        /// end test hard code
    }

    void KGEDevice::SoftRasterization_solid(HDC hdc)
    {

    }

    void KGEDevice::SoftRasterization_edge(HDC hdc)
    {

    }

}; // end of namespace KGE
