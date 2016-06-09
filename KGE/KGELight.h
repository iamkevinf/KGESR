#ifndef __KGE_LIGHT_H__
#define __KGE_LIGHT_H__

#include "KGEMath.h"

namespace KGE
{

    class KGELight
    {
    public:
        KGELight();
        ~KGELight();

        void init(const Vector4 & _ambient, const Vector4 & _diffuse, const Vector4 & specular, float _shniness);

    private:
        Vector4 ambient;
        Vector4 diffuse;
        Vector4 specular;

        Vector4 pos;
    };

}; // end of namespace KGE

#endif // __KGE_LIGHT_H__

