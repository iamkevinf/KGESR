#include "stdafx.h"
#include "KGECamera.h"

#include <assert.h>

namespace KGE
{
    KGECamera::KGECamera(float w, float h) :
        fov(60),
        aspect(w/h),
        zNear(0.5f),
        zFar(150),
        projectionMode(ProjectionMode::PerspectiveMode),
        rect(-w / 2, -w / 2 + w, -h / 2, -h / 2 + h, 1024, 1024)
    {
    }


    KGECamera::~KGECamera()
    {

    }

    Matrix KGECamera::View()const
    {
        Matrix mat;
        mat = ViewMaterix(eyePos, center, up);
        return mat;
    }

    Matrix KGECamera::InverseView()const
    {
        Matrix mat;
        mat = ViewMaterixInverse(eyePos, center, up);
        return mat;
    }

    Matrix KGECamera::Projection()const
    {
        Matrix mat;
        switch (projectionMode)
        {
        case ProjectionMode::OrthographicMode:
            mat = OrthoprojectionMatrix(rect.getMinX(), rect.getMaxX(), rect.getMinY(), rect.getMaxY(), rect.getMinZ(), rect.getMaxZ());
            break;
        case ProjectionMode::PerspectiveMode:
            mat = PrespectiveProjectionMatrix(fov, aspect, zNear, zFar);
            break;
        default:
            assert(false);
            break;
        }

        return mat;
    }

    void KGECamera::rotate(const Vector4 & axis, float angle)
    {
        Matrix view = View();
        Matrix invView = InverseView();
        Matrix rotMat = MatrixRotation(axis, angle);
        Matrix mat = invView * rotMat;

        Vector4 _up = Vector4(mat.at(4), mat.at(5), mat.at(6), mat.at(7));
        Vector4 look = Vector4(mat.at(8), mat.at(9), mat.at(10), mat.at(11)*-1);
        Vector4 pos = Vector4(mat.at(12), mat.at(13), mat.at(14), mat.at(15));

        _up.w = 0;
        look.w = 0;
        pos.w = 1;

        eyePos = pos;
        center = pos + look;
        up = _up;
    }
} // end of namespace KGE
