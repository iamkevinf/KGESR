#include "stdafx.h"
#include "Vector2.h"


Vector2::Vector2():
x(0),
y(0)
{
}

Vector2::Vector2(float _x, float _y) :
x(_x),
y(_y)
{

}


Vector2::~Vector2()
{
}

Vector2 Vector2::operator+(const Vector2 & rhs)const
{
    return Vector2(x + rhs.x, y + rhs.y);
}

Vector2 Vector2::operator-(const Vector2 & rhs)const
{
    return Vector2(x - rhs.x, y - rhs.y);
}

Vector2 Vector2::operator*(const Vector2 & rhs)const
{
    return Vector2(x * rhs.x, y * rhs.y);
}

