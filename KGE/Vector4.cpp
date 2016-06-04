#include "stdafx.h"
#include "Vector4.h"


Vector4::Vector4() : Vector4(0, 0, 0, 0)
{

}

Vector4::Vector4(float _x, float _y, float _z, float _w) :
x(_x),
y(_y),
z(_z),
w(_w)
{
    _array[0] = x;
    _array[1] = y;
    _array[2] = z;
    _array[3] = w;
}


Vector4::~Vector4()
{

}

Vector4 Vector4::operator+(const Vector4 & rhs) const
{
    return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

Vector4 Vector4::operator-(const Vector4 & rhs) const
{
    return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

Vector4 Vector4::operator*(float k)const
{
    return Vector4(x*k, y*k, z*k, w*k);
}


