#include "stdafx.h"
#include "KGETriangle.h"

#include <assert.h>

namespace KGE
{

    KGETriangle::KGETriangle()
    {
        memset(&_array, 0, sizeof(_array));
    }

    KGETriangle::KGETriangle(int ID0, int ID1, int ID2)
    {
        _array[0] = ID0;
        _array[1] = ID1;
        _array[2] = ID2;
    }


    KGETriangle::~KGETriangle()
    {
    }

    short KGETriangle::vID(int i)const
    {
        assert(i >= 0 && i <= 3);
        return _array[i];
    }

    void KGETriangle::SetVID(int i, short vID)
    {
        assert(i >= 0 && i <= 3);
        _array[i] = vID;
    }

}; // end of namespace KGE
