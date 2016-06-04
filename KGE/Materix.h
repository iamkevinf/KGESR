#ifndef __KGE_MATERIX_H__
#define __KGE_MATERIX_H__

#include "Vector4.h"

class Materix
{
public:
    Materix();
    Materix(const float array[16]);
    Materix(
        float m11, float m12, float m13, float m14,
        float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34,
        float m41, float m42, float m43, float m44
        );
    ~Materix();

    const float * getArray() const { return _array; }

    float at(int index)const;
    void at(int index, float value);

    Materix operator*(const Materix & rhs)const;
    Materix operator*(const Vector4 & v)const;
    Materix operator*(float k)const;
    Materix operator+(const Materix & rhs)const;

private:
    float _array[16];
};

#endif // __KGE_MATERIX_H__

