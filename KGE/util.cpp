#include "stdafx.h"
#include "util.h"

namespace KGE
{
    void numberToStr(int num, std::string & str)
    {
        char numbuf[128];
        ::sprintf_s(numbuf, "%i", num);
        str = numbuf;
    }
}
