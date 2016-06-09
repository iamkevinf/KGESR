#ifndef __KGE_CAMERA_H__
#define __KGE_CAMERA_H__

#include "KGEMath.h"

enum class ProjectionMode
{
    UnKnow = 0,
    OrthographicMode,
    PerspectiveMode,
    MAX
};

namespace KGE
{
    class KGECamera
    {
    public:
        KGECamera(float w, float h);
        ~KGECamera();

        Matrix View()const;
        Matrix InverseView()const;
        Matrix Projection()const;

        void rotate(const Vector4 & axis, float angle);

        inline void init(const Vector4 & _pos, const Vector4 & _center, const Vector4 & _up) { eyePos = _pos; center = _center; up = _up; }

    private:
        KGECamera();

    protected:
        float fov;
        float aspect;
        float zNear;
        float zFar;

        ProjectionMode projectionMode;

        Vector4 eyePos;
        Vector4 center;
        Vector4 up;

        Vector4 rect;

    };
} // end of namespce KGE

#endif // __KGE_CAMERA_H__

