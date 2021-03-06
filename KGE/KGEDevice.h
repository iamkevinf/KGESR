#ifndef __KGE_DEVICE_H__
#define __KGE_DEVICE_H__

#include "KGEMath.h"
#include "KGEMesh.h"
#include "KGECamera.h"
#include "KGELight.h"
#include "KGEVertex.h"

namespace KGE
{
    enum class DrawMode
    {
        DrawSolid = 0,
        DrawEdge,
        DrawFull,

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
        void DrawTriangle(HDC, int x1, int y1, int x2, int y2, int x3, int y3, const Vector4 & color);
        void DrawTriangle_Full(HDC, int x1, int y1, int x2, int y2, int x3, int y3, const Vector4 & color);
        void DrawTriangle_Edge(HDC, int x1, int y1, int x2, int y2, int x3, int y3, const Vector4 & color);
        void DrawTriangle_Solid(HDC, int x1, int y1, int x2, int y2, int x3, int y3, const Vector4 & color);
        /// 平顶三角形
        void _DrawTriangle_flat(HDC, int x1, int y1, int x2, int y2, int x3, int y3, const Vector4 & color);
        /// 平底三角形
        void _DrawTriangle_loafer(HDC, int x1, int y1, int x2, int y2, int x3, int y3, const Vector4 & color);

        void Clear(HDC hdc, const Vector4 & viewpoint, DWORD color);
        void Present(HDC hdc, HDC hdcSrc, const Vector4 & viewpoint);

        void ViewPort(Vector4 & viewport);

        void SoftRasterization(HDC hdc);

        void ZBufferWrite(int x, int y, double value);
        double ZBufferRead(int x, int y);
        void ZBufferClear();

        KGEVertex VertexShaderProgram(const Matrix & mat, const KGECamera * camera, const KGELight * light, const KGEMaterial * material, const KGEVertex & v);
        KGEFragment FragmentShaderProgram(const KGEVertex & interpolatedV, const KGETexture * texture);

    private:
        void submitMesh();
        void transformVertexes();

    protected:
        void SoftRasterization_solid(HDC hdc);
        void SoftRasterization_edge(HDC hdc);

    private:
        Vector4 _viewport;
        double * _zBuffer;

        bool _isInited;

        KGEMesh * _mesh;
        KGEMesh * _transformMesh;
        KGECamera * _camera;
        KGELight * _light;

        DrawMode _drawMode;

        Matrix _mat;
        Matrix _viewportMat;
    };

}; // end of namespace KGE

#endif // __KGE_DEVICE_H__
