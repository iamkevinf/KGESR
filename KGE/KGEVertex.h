#ifndef __KGE_VERTEX_H__
#define __KGE_VERTEX_H__

#include "KGEMath.h"

namespace KGE
{
    class KGEVertex
    {
    public:
        KGEVertex();
        ~KGEVertex();

    public:
        Vector4 pos;
        Vector4 norm;
        Vector4 color;
        Vector4 colorAdd;
        Vector2 uv;
        bool isCullBack;
        int textureID;
        int materialID;
    };

    class KGEFragment
    {
    public:
        KGEFragment();
        ~KGEFragment();

        Vector4 pos;
        Vector4 color;
    };

}; // end of namespace KGE

#endif // __KGE_VERTEX_H__

