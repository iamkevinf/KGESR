#ifndef __KGE_DEVICE_H__
#define __KGE_DEVICE_H__

#include "KGEMath.h"

namespace KGE
{
    enum class DrawMode
    {
        DrawSolid = 0,
        DrawEdge,

        DrawModeMax
    };

    class KGEDevice
    {
    public:
        KGEDevice();
        ~KGEDevice();

        bool init(const Vector4 & viewport);
        bool GetIsInited(){ return _isInited; }

        void DrawPoint(HDC hdc, int x, int y, const Vector4 & color);
        // draw a line with DDA
        void DrawLine(HDC hdc, int x, int y, int xEnd, int yEnd, const Vector4 & color);

        void Clear(HDC hdc, const Vector4 & viewpoint, DWORD color);
        void Present(HDC hdc, HDC hdcSrc, const Vector4 & viewpoint);

        void ViewPort(Vector4 & viewport);

        void SoftRasterization(HDC hdc);

    protected:
        void SoftRasterization_solid(HDC hdc);
        void SoftRasterization_edge(HDC hdc);

    private:
        Vector4 _viewport;

        bool _isInited;
    };

}; // end of namespace KGE

#endif // __KGE_DEVICE_H__
