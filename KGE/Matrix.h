#ifndef __KGE_MATERIX_H__
#define __KGE_MATERIX_H__

#include "Vector4.h"

class Matrix
{
public:
    Matrix();
    Matrix(const float array[16]);
    Matrix(
        float m11, float m12, float m13, float m14,
        float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34,
        float m41, float m42, float m43, float m44
        );
    ~Matrix();

    const float * getArray() const { return _array; }

    float at(int index)const;
    void at(int index, float value);

    Matrix operator*(const Matrix & rhs)const;
    Matrix operator*(const Vector4 & v)const;
    Matrix operator*(float k)const;
    Matrix operator+(const Matrix & rhs)const;

private:
    float _array[16];
};

Matrix ViewMaterixInverse(const Vector4 & eyePos, const Vector4 & center, const Vector4 & up);
Matrix ViewMaterix(const Vector4 & eyePos, const Vector4 & center, const Vector4 & up);
Matrix MaterixRotation(const Vector4 & axis, float angle);
Matrix MatrixScale(float x, float y, float z);
Matrix MaterixTranslate(const Vector4 & translate);

Matrix PrespectiveProjectionMatrix(float fov, float aspect, float zNear, float zFar);
Matrix OrthoprojectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar);

#endif // __KGE_MATERIX_H__

