#include "stdafx.h"
#include "Matrix.h"

#include <math.h>

#include <assert.h>

#include "KGEMath.h"

Matrix::Matrix()
{
    memset(&_array, 0, sizeof(_array));
}

Matrix::Matrix(
    float m11, float m12, float m13, float m14,
    float m21, float m22, float m23, float m24,
    float m31, float m32, float m33, float m34,
    float m41, float m42, float m43, float m44
    )
    :Matrix()
{
    _array[0] = m11; _array[1] = m12; _array[2] = m13; _array[3] = m14;
    _array[4] = m21; _array[5] = m22; _array[6] = m23; _array[7] = m24;
    _array[8] = m31; _array[9] = m32; _array[10] = m33; _array[11] = m34;
    _array[12] = m41; _array[13] = m42; _array[14] = m43; _array[15] = m44;
}

Matrix::Matrix(const float array[16])
{
    for (int i = 0; i < 16; ++i)
        _array[i] = array[i];
}

Matrix::~Matrix()
{
    delete[] _array;
}

float Matrix::at(int index)const
{
    assert(index >= 0 && index <= 15);
    return _array[index];
}

void Matrix::at(int index, float value)
{
    assert(index >= 0 && index <= 15);
    _array[index] = value;
}

Matrix Matrix::operator+(const Matrix & rhs)const
{
    const float * a = getArray();
    const float * b = rhs.getArray();

    float r[16];

    for (int i = 0; i < 16; ++i)
        r[i] = a[i] + b[i];

    return Matrix(r);
}

Matrix Matrix::operator*(const Matrix & rhs)const
{
    const float * a = getArray();
    const float * b = rhs.getArray();

    float r[16];


    r[0] = b[0] * a[0] + b[1] * a[4] + b[2] * a[8] + b[3] * a[12];
    r[1] = b[0] * a[1] + b[1] * a[5] + b[2] * a[9] + b[3] * a[13];
    r[2] = b[0] * a[2] + b[1] * a[6] + b[2] * a[10] + b[3] * a[14];
    r[3] = b[0] * a[3] + b[1] * a[7] + b[2] * a[11] + b[3] * a[15];

    r[4] = b[4] * a[0] + b[5] * a[4] + b[6] * a[8] + b[7] * a[12];
    r[5] = b[4] * a[1] + b[5] * a[5] + b[6] * a[9] + b[7] * a[13];
    r[6] = b[4] * a[2] + b[5] * a[6] + b[6] * a[10] + b[7] * a[14];
    r[7] = b[4] * a[3] + b[5] * a[7] + b[6] * a[11] + b[7] * a[15];

    r[8] = b[8] * a[0] + b[9] * a[4] + b[10] * a[8] + b[11] * a[12];
    r[9] = b[8] * a[1] + b[9] * a[5] + b[10] * a[9] + b[11] * a[13];
    r[10] = b[8] * a[2] + b[9] * a[6] + b[10] * a[10] + b[11] * a[14];
    r[11] = b[8] * a[3] + b[9] * a[7] + b[10] * a[11] + b[11] * a[15];

    r[12] = b[12] * a[0] + b[13] * a[4] + b[14] * a[8] + b[15] * a[12];
    r[13] = b[12] * a[1] + b[13] * a[5] + b[14] * a[9] + b[15] * a[13];
    r[14] = b[12] * a[2] + b[13] * a[6] + b[14] * a[10] + b[15] * a[14];
    r[15] = b[12] * a[3] + b[13] * a[7] + b[14] * a[11] + b[15] * a[15];

    return Matrix(r);
}

Vector4 Matrix::operator*(const Vector4 & v)const
{
    const float *m = this->getArray();
    const float vinx = v.x;
    const float viny = v.y;
    const float vinz = v.z;
    const float vinw = v.w;
    float vout[4];
    vout[0] = vinx * m[0] + viny * m[4] + vinz * m[8] + vinw * m[12];
    vout[1] = vinx * m[1] + viny * m[5] + vinz * m[9] + vinw * m[13];
    vout[2] = vinx * m[2] + viny * m[6] + vinz * m[10] + vinw * m[14];
    vout[3] = vinx * m[3] + viny * m[7] + vinz * m[11] + vinw * m[15];
    return Vector4(vout[0], vout[1], vout[2], vout[3]);
}

Matrix Matrix::operator*(float k)const
{
    const float * m = getArray();
    float r[16];

    for (int i = 0; i < 16; ++i)
        r[i] = m[i] * k;

    return Matrix(r);
}

Matrix ViewMaterixInverse(const Vector4 & eyePos, const Vector4 & center, const Vector4 & _up)
{
    assert(eyePos.w == 1);
    assert(center.w == 1);
    assert(_up.w == 1);

    Vector4 forward, right, up;

    forward = (center - eyePos);
    forward.normalized();
    up = _up;

    right = cross(forward, up);
    right.normalized();
    up = cross(right, forward);

    Vector4 back = Vector4() - forward;

    float m[16] = 
    {
        right.x, right.y, right.z, 0,
        up.x, up.y, up.z, 0,
        back.x, back.y, back.z, 0,
        eyePos.x, eyePos.y, eyePos.z, 1
    };

    Matrix viewMatInverse(m);

    return viewMatInverse;
}

Matrix ViewMaterix(const Vector4 & eyePos, const Vector4 & center, const Vector4 & up)
{
    assert(eyePos.w == 1);
    assert(center.w == 1);
    assert(up.w == 1);

    Matrix viewMatInverse = ViewMaterixInverse(eyePos, center, up);


    const float *mInv = viewMatInverse.getArray();

    float x = -(mInv[12] * mInv[0] + mInv[13] * mInv[1] + mInv[14] * mInv[2]);
    float y = -(mInv[12] * mInv[4] + mInv[13] * mInv[5] + mInv[14] * mInv[6]);
    float z = -(mInv[12] * mInv[8] + mInv[13] * mInv[9] + mInv[14] * mInv[10]);

    float m[16] = {
        mInv[0], mInv[4], mInv[8], 0,
        mInv[1], mInv[5], mInv[9], 0,
        mInv[2], mInv[6], mInv[10], 0,
        x, y, z, 1
    };

    return Matrix(m);
}

Matrix MaterixRotation(const Vector4 & axis, float angle)
{
    assert(axis.w == 0);

    float nx = axis.x;
    float ny = axis.y;
    float nz = axis.z;

    float cosA = cos(angle);
    float sinA = sin(angle);

    float _cosA = 1 - cosA;
    
    float nx_cosA = nx * _cosA;
    float nz_cosA = nz * _cosA;

    float nxnx_cosA = nx * nx_cosA;
    float nxny_cosA = ny * nx_cosA;
    float nxnz_cosA = nz * nx_cosA;

    float nyny_cosA = ny * ny * _cosA;
    float nynz_cosA = ny * ny * _cosA;

    float nznz_cosA = nz * nz * _cosA;

    float nxsinA = nx * sinA;
    float nzsinA = nz * sinA;
    float nysinA = ny * sinA;

    float rotMat[16] = {
        nxnx_cosA + cosA, nxny_cosA + nzsinA, nxnz_cosA - nysinA, 0,//col 1
        nxny_cosA - nzsinA, nyny_cosA + cosA, nynz_cosA + nxsinA, 0,//col 2
        nxnz_cosA + nysinA, nynz_cosA - nxsinA, nznz_cosA + cosA, 0,//col 3
        0, 0, 0, 1//col 4
    };

    return Matrix(rotMat);
}

Matrix MatrixScale(float x, float y, float z)
{
    float mat[16] = {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };


    return Matrix(mat);
}

Matrix MaterixTranslate(const Vector4 & transform)
{
    assert(transform.w == 0);

    float x = transform.x;
    float y = transform.y;
    float z = transform.z;

    float mat[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };

    return Matrix(mat);
}

Matrix PrespectiveProjectionMatrix(float fov, float aspect, float zNear, float zFar)
{
    double sine, cotangent, deltaZ;
    double radians = (fov / 2) * M_PI / 180;

    deltaZ = zFar - zNear;
    sine = sinf(radians);

    if ((deltaZ == 0) || (sine == 0) || (aspect == 0))
        return zeroMat();

    cotangent = cosf(radians) / sine;

    float m[16] = {
        cotangent / aspect, 0, 0, 0,//col 1
        0, cotangent, 0, 0,//col 2
        0, 0, -(zFar + zNear) / deltaZ, -1,//col 3
        0, 0, -2 * zNear * zFar / deltaZ, 0 };//col 4

    return Matrix(m);
}

Matrix OrthoprojectionMatrix(float left, float right, float bottom, float top, float _near, float _far)
{
    //"near" and "far" have been defined in windows, so we use other var name 

    float tx = -((right + left) / (right - left));
    float ty = -((top + bottom) / (top - bottom));
    float tz = -((_far + _near) / (_far - _near));

    float m[16] = {
        2.0 / (right - left), 0, 0, 0,//col 1
        0, 2.0 / (top - bottom), 0, 0,//col 2
        0, 0, (-2.0) / (_far - _near), 0,//col 3
        tx, ty, tz, 1//col 4
    };

    return Matrix(m);
}

Matrix transpose(const Matrix & mat)
{
    const float* m = mat.getArray();

    float rs[16] = {
        m[0], m[4], m[8], m[12],//col 1
        m[1], m[5], m[9], m[13],//col 2
        m[2], m[6], m[10], m[14],//col 3
        m[3], m[7], m[11], m[15]//col 4
    };

    return Matrix(rs);
}

Matrix inverse(const Matrix & mat)
{
    float m[16];
    for (int i = 0; i<16; i++)m[i] = mat.at(i);

    float invOut[16];
    float inv[16], det;
    int i;

    inv[0] = m[5] * m[10] * m[15] -
        m[5] * m[11] * m[14] -
        m[9] * m[6] * m[15] +
        m[9] * m[7] * m[14] +
        m[13] * m[6] * m[11] -
        m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] +
        m[4] * m[11] * m[14] +
        m[8] * m[6] * m[15] -
        m[8] * m[7] * m[14] -
        m[12] * m[6] * m[11] +
        m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] -
        m[4] * m[11] * m[13] -
        m[8] * m[5] * m[15] +
        m[8] * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] +
        m[4] * m[10] * m[13] +
        m[8] * m[5] * m[14] -
        m[8] * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] +
        m[1] * m[11] * m[14] +
        m[9] * m[2] * m[15] -
        m[9] * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] -
        m[0] * m[11] * m[14] -
        m[8] * m[2] * m[15] +
        m[8] * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] +
        m[0] * m[11] * m[13] +
        m[8] * m[1] * m[15] -
        m[8] * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] -
        m[0] * m[10] * m[13] -
        m[8] * m[1] * m[14] +
        m[8] * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] -
        m[1] * m[7] * m[14] -
        m[5] * m[2] * m[15] +
        m[5] * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] +
        m[0] * m[7] * m[14] +
        m[4] * m[2] * m[15] -
        m[4] * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] -
        m[0] * m[7] * m[13] -
        m[4] * m[1] * m[15] +
        m[4] * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] +
        m[0] * m[6] * m[13] +
        m[4] * m[1] * m[14] -
        m[4] * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        //return false;
        assert(false);

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return Matrix(invOut);
}

Vector4 perComponentProduct(const Vector4&v1, const Vector4&v2){
    Vector4 rs = Vector4(
        v1.x*v2.x,
        v1.y*v2.y,
        v1.z*v2.z,
        v1.w*v2.w);
    return rs;
}
