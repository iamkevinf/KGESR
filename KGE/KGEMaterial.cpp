#include "stdafx.h"
#include "KGEMaterial.h"

namespace KGE
{

    KGEMaterial::KGEMaterial():
        ambient(0.2f, 0.2f, 0.2f, 1.f),
        diffuse(0.8f, 0.8f, 0.8f, 1.f),
        specular(0, 0, 0, 1),
        shininess(2)
    {
    }


    KGEMaterial::~KGEMaterial()
    {
    }

}; // end of namespace KGE
