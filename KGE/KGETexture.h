#ifndef __KGE_TEXTURE_H__
#define __KGE_TEXTURE_H__

#include "KGEMath.h"
#include "TextureLoader.h"

#include <string>

namespace KGE
{

    class KGETexture
    {
    public:
        KGETexture();
        ~KGETexture();

        bool LoadFromFile(const std::string & filename);

        Vector4 GetColor(const Vector2 & uv)const;

    protected:
        TextureLoader * textureLoader;
    };

}; // end of namespace KGE

#endif // __KGE_TEXTURE_H__

