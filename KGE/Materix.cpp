#include "stdafx.h"
#include "Materix.h"

#include <assert.h>

Materix::Materix()
{
    memset(&_array, 0, sizeof(_array));
}

Materix::Materix(
    float m11, float m12, float m13, float m14,
    float m21, float m22, float m23, float m24,
    float m31, float m32, float m33, float m34,
    float m41, float m42, float m43, float m44
    )
    :Materix()
{
    _array[0] = m11; _array[1] = m12; _array[2] = m13; _array[3] = m14;
    _array[4] = m21; _array[5] = m22; _array[6] = m23; _array[7] = m24;
    _array[8] = m31; _array[9] = m32; _array[10] = m33; _array[11] = m34;
    _array[12] = m41; _array[13] = m42; _array[14] = m43; _array[15] = m44;
}

Materix::Materix(const float array[16])
{
    for (int i = 0; i < 16; ++i)
        _array[i] = array[i];
}

Materix::~Materix()
{
    delete[] _array;
}

float Materix::at(int index)const
{
    assert(index >= 0 && index <= 15);
    return _array[index];
}

void Materix::at(int index, float value)
{
    assert(index >= 0 && index <= 15);
    _array[index] = value;
}

Materix Materix::operator+(const Materix & rhs)const
{
    const float * a = getArray();
    const float * b = rhs.getArray();

    float r[16];

    for (int i = 0; i < 16; ++i)
        r[i] = a[i] + b[i];

    return Materix(r);
}

Materix Materix::operator*(const Materix & rhs)const
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

    return Materix(r);
}

Materix Materix::operator*(const Vector4 & v)const
{
    const float *m = this->getArray();
    const float *vin = v.getArray();
    float vout[4];
    vout[0] = vin[0] * m[0] + vin[1] * m[4] + vin[2] * m[8] + vin[3] * m[12];
    vout[1] = vin[0] * m[1] + vin[1] * m[5] + vin[2] * m[9] + vin[3] * m[13];
    vout[2] = vin[0] * m[2] + vin[1] * m[6] + vin[2] * m[10] + vin[3] * m[14];
    vout[3] = vin[0] * m[3] + vin[1] * m[7] + vin[2] * m[11] + vin[3] * m[15];
    return Materix(vout);
}

Materix Materix::operator*(float k)const
{
    const float * m = getArray();
    float r[16];

    for (int i = 0; i < 16; ++i)
        r[i] = m[i] * k;

    return Materix(r);
}
