#ifndef __KGE_MATH_H__
#define __KGE_MATH_H__

#include <math.h>

#include "Vector4.h"
#include "Vector2.h"
#include "Matrix.h"
#include "Range.h"

const float M_PI = 3.1415927f;

inline Matrix zeroMat()
{
    Matrix mat(0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0);

    return mat;
}

inline Matrix identityMatrix()
{
    Matrix mat(1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
    return mat;
}

#endif // __KGE_MATH_H__