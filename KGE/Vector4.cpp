#include "stdafx.h"
#include "Vector4.h"

#include <math.h>

#include <assert.h>

Vector4::Vector4() : Vector4(0, 0, 0, 0)
{

}

Vector4::Vector4(float _x, float _y, float _z, float _w) :
x(_x),
y(_y),
z(_z),
w(_w)
{
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

void Vector4::persDiv()
{
    if (w == 0)
        return;

    x /= w;
    y /= w;
    z /= w;
    w = 1;
}

void Vector4::normalized()
{
    float r2 = x*x + y*y + z*z;
    if (r2 == 0)
    {
        x = y = z = w = 0;
    }
    else
    {
        float r = sqrtf(r2);

        x = x / r;
        y = y / r;
        z = z / r;
        w = 0;
    }
}

float Vector4::distance()
{
    assert(w == 0);
    
    return sqrtf(x*x + y*y + z*z);
}

float Vector4::distanceSqr()
{
    assert(w == 0);

    return x*x + y*y + z*z;
}

Vector4 cross(const Vector4 & lhs, const Vector4 & rhs)
{
    assert(lhs.w == 0);
    assert(rhs.w == 0);

    return Vector4(
        lhs.y*rhs.z - lhs.z*rhs.y,
        lhs.z*rhs.x - lhs.x*rhs.z,
        lhs.x*rhs.y - lhs.y*rhs.x,
        0
        );
}

float dot(const Vector4 & lhs, const Vector4 & rhs)
{
    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

