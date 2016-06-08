#ifndef __KGE_UTIL_H__
#define __KGE_UTIL_H__

#include <string>
#include <vector>

#include "KGEMath.h"

namespace KGE
{
    void numberToStr(int num, std::string & str);
    int charToNumber(char c);
    int strToNumber(const std::string&numStr);

    std::vector<std::string> divideStr(const std::string&str, const std::string&dividerChars);
    Vector4 fscanVector4(FILE * fp);
    Vector4 fscanVector3(FILE * fp);
    Vector2 fscanVector2(FILE * fp);
    float fscanFloat(FILE * fp);
    int fscanInt(FILE * fp);
    std::string fscanStr(FILE*fp, const std::string & expectedStr = "");
    std::string fscanOneQuotation(FILE * fp);
}

#endif // __KGE_UTIL_H__
