#ifndef __KGE_VECTOR2_H__
#define __KGE_VECTOR2_H__

class Vector2
{
public:
    Vector2();
    Vector2(float x, float y);
    ~Vector2();

    Vector2 operator+(const Vector2 & rhs)const;
    Vector2 operator-(const Vector2 & rhs)const;
    Vector2 operator*(const Vector2 & rhs)const;

    float x;
    float y;
};

#endif // __KGE_VECTOR2_H__

