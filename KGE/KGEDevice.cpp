#include "stdafx.h"
#include "KGEDevice.h"

#include "KGEMath.h"
#include "KGEMesh.h"
#include "KGEVertex.h"
#include "KGETexture.h"

#include "KGEMaterial.h"
#include "KGELight.h"

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

        ViewportMatrix(viewport, _viewportMat);

        _light = new KGELight();
        _light->pos = Vector4(-50 * 10, 100 * 10, 70 * 10, 1);
        _light->ambient = Vector4(1, 1, 1, 1);

        _zBuffer = new double[(int)viewport.z * (int)viewport.w];
        ZBufferClear();

        _mesh = new KGEMesh();
        _mesh->LoadFromFile("res/ASEModels/", "teaport.ASE");

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
    /// Shader
    ////////////////////////////////////////////////////////

    KGEVertex KGEDevice::VertexShaderProgram(const Matrix & mat, const KGECamera * camera, const KGELight * light, const KGEMaterial * material, const KGEVertex & v)
    {
        const Matrix & viewMat = camera->View();
        const Matrix & projMat = camera->Projection();
        const Matrix & normMat = transpose(inverse(mat));
        const Vector4 & ambientML = perComponentProduct(light->ambient, material->ambient);
        const Vector4 & diffuseML = perComponentProduct(light->diffuse, material->diffuse);
        const Vector4 & specularML = perComponentProduct(light->specular, material->specular);
        const float shininess = material->shininess;
        //----transform pos
        Vector4 pos_world = mat*v.pos;
        //----transform norm
        Vector4 norm_world = (normMat*v.norm);
        norm_world.normalized();
        //----calculate lighting
        Vector4 posToLight = (light->pos - pos_world);
        posToLight.normalized();
        float normDotPosToLight = max(0.0, dot(norm_world, posToLight));
        Vector4 diffuseColor = diffuseML*normDotPosToLight;
        diffuseColor.w = diffuseML.w;
        //----calculate ambient
        Vector4 ambientColor = ambientML;
        ambientColor.w = 1;
        //----calculate specular

        Vector4 eyePos_world = camera->eyePos;
        Vector4 posToEye = eyePos_world - pos_world;
        posToEye.normalized();

        Vector4 halfVector = (posToLight + posToEye)*0.5;
        float normDotHalfVector = dot(norm_world, halfVector);
        float pf = normDotHalfVector == 0 ? 0 : pow(normDotHalfVector, shininess);

        Vector4 specularColor = specularML*pf;
        specularColor.w = 1;

        //----calculate mainColor
        Vector4 mainColor = diffuseColor + ambientColor;
        Vector4 colorAdd = specularColor;

        //----gl_Position
        Vector4 gl_Position = projMat*viewMat*pos_world;
        //----make vsOutput
        KGEVertex transformedV = v;//copy v
        transformedV.pos = gl_Position;
        transformedV.color = mainColor;
        transformedV.colorAdd = colorAdd;
        return transformedV;
    }

    KGEFragment KGEDevice::FragmentShaderProgram(const KGEVertex & interpolatedV, const KGETexture * texture)
    {
        KGEFragment frag;
        frag.pos = interpolatedV.pos;
        frag.color = perComponentProduct(interpolatedV.color, texture->GetColor(interpolatedV.uv));
        frag.color = frag.color + interpolatedV.colorAdd;//must add after mul textureColor
        return frag;
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

    void KGEDevice::submitMesh()
    {
        _transformMesh = _mesh;
    }

    void KGEDevice::transformVertexes()
    {
        int vertexCount = (int)_transformMesh->positionList.size();
        for (int i = 0; i < vertexCount; ++i)
        {
            KGEVertex v = _transformMesh->GetVertex(i);
            _mat = identityMatrix();
            KGEVertex tranformedV = VertexShaderProgram(_mat, _camera, _light, _transformMesh->materialList[v.materialID], v);

            _transformMesh->SetVertex(i, tranformedV);
        }


        vertexCount = _transformMesh->positionList.size();

        for (int i = 0; i < vertexCount; ++i)
        {
            _transformMesh->positionList[i].persDiv();
            _transformMesh->positionList[i] = _viewportMat * _transformMesh->positionList[i];
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
        submitMesh();

        switch (_drawMode)
        {
        case DrawMode::DrawEdge:
            break;
        case DrawMode::DrawSolid:
            break;
        }
        /// TODO
        /// for test hard code
        DrawPoint(hdc, 100, 100, Vector4(1, 0, 0, 1));
        DrawLine(hdc, 200, 247, 300, 405, Vector4(1, 0, 0, 1));
        DrawTriangle(hdc, 500, 200, 400, 300, 700, 400, Vector4(0, 1, 0, 1));

        int nIDtri = (int)_transformMesh->triList.size();
        for (int i = 0; i<nIDtri; i++){
            const KGETriangle & IDtri = _mesh->triList[i];
            const int vID0 = IDtri.vID(0);
            const int vID1 = IDtri.vID(1);
            const int vID2 = IDtri.vID(2);
            if (vID0 == -1)continue;
            KGEVertex v0, v1, v2;
            v0 = _transformMesh->GetVertex(vID0);
            v1 = _transformMesh->GetVertex(vID1);
            v2 = _transformMesh->GetVertex(vID2);

            DrawTriangle(hdc, v0.pos.x, v0.pos.y, v1.pos.x, v1.pos.y, v2.pos.x, v2.pos.y, v0.color);
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
