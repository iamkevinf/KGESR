#include "stdafx.h"
#include "KGELight.h"

namespace KGE
{

    KGELight::KGELight() :
        ambient(0.2f, 0.2f, 0.2f, 1.f),
        diffuse(0.8f, 0.8f, 0.8f, 1.f),
        specular(0, 0, 0, 1)
    {
    }


    KGELight::~KGELight()
    {
    }

    void KGELight::init(const Vector4 & _ambient, const Vector4 & _diffuse, const Vector4 & _specular, float _shniness)
    {
        ambient = _ambient;
        diffuse = _diffuse;
        specular = _specular;
    }

}; // end of namespace KGE
