#ifndef __KGE_VECTOR4_H__
#define __KGE_VECTOR4_H__

class Vector4
{
public:
    Vector4();
    Vector4(float x, float y, float z, float w);
    ~Vector4();

    float x, y, z, w;

    Vector4 operator + (const Vector4 & rhs) const;
    Vector4 operator - (const Vector4 & rhs) const;
    Vector4 operator * (float k) const;

    void persDiv();

    void normalized();

    float distance();
    float distanceSqr();
};

Vector4 cross(const Vector4 & lhs, const Vector4 & rhs);
float dot(const Vector4 & lhs, const Vector4 & rhs);

#endif // __KGE_VECTOR4_H__

