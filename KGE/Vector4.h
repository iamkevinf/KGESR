#ifndef __KGE_VECTOR4_H__
#define __KGE_VECTOR4_H__

class Vector4
{
public:
    Vector4();
    Vector4(float x, float y, float z, float w);
    ~Vector4();

    float x, y, z, w;

    const float* getArray()const{ return _array; }

    Vector4 operator + (const Vector4 & rhs) const;
    Vector4 operator - (const Vector4 & rhs) const;
    Vector4 operator * (float k) const;

private:
    float _array[4];
};


#endif // __KGE_VECTOR4_H__

