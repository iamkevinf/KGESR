#ifndef __KGE_MATERIAL_H__
#define __KGE_MATERIAL_H__

#include "KGEMath.h"

namespace KGE
{

    class KGEMaterial
    {
    public:
        KGEMaterial();
        ~KGEMaterial();

        void init(const Vector4 & _ambient, const Vector4 & _diffuse, const Vector4 & specular, float _shniness);

    private:
        Vector4 ambient;
        Vector4 diffuse;
        Vector4 specular;

        float shininess;
    };

}; // end of namespace KGE

#endif // __KGE_MATERIAL_H__

