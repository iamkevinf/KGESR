#include "stdafx.h"
#include "KGETexture.h"

namespace KGE
{

    KGETexture::KGETexture():
        textureLoader(nullptr)
    {
    }


    KGETexture::~KGETexture()
    {
        if (textureLoader)
        {
            textureLoader = nullptr;
            delete textureLoader;
        }
    }

    bool KGETexture::LoadFromFile(const std::string & filename)
    {
        textureLoader = new TextureLoader();
        return textureLoader->ReadBmp(filename);
    }

    Vector4 KGETexture::GetColor(const Vector2 & uv)const
    {
        float u = uv.x;
        float v = uv.y;

        u = u - floor(u);
        v = v - floor(v);

        float j = u * textureLoader->width;
        float i = v * textureLoader->height;

        Vector4 color;
        float jL, jR, iU, iD;
        jL = floor(j - 0.5) + 0.5;
        jR = jL + 1;
        iU = floor(i - 0.5) + 0.5;
        iD = iU + 1;
        float djL = j - jL;
        float djR = jR - j;
        float diU = i - iU;
        float diD = iD - i;
        int jL_int = floor(jL);
        int jR_int = jL_int + 1;
        int iU_int = floor(iU);
        int iD_int = iU_int + 1;
        ColorRGB LUcolor = textureLoader->GetPixelColor(Vector2(max(iU_int, 0), max(jL_int, 0)));
        ColorRGB LDcolor = textureLoader->GetPixelColor(Vector2(min(iD_int, textureLoader->height - 1), max(jL_int, 0)));
        ColorRGB RUcolor = textureLoader->GetPixelColor(Vector2(max(iU_int, 0), min(jR_int, textureLoader->width - 1)));
        ColorRGB RDcolor = textureLoader->GetPixelColor(Vector2(min(iD_int, textureLoader->height - 1), min(jR_int, textureLoader->width - 1)));
        Vector4 LUcolorf((float)LUcolor.r / 255, (float)LUcolor.g / 255, (float)LUcolor.b / 255, 1);
        Vector4 LDcolorf((float)LDcolor.r / 255, (float)LDcolor.g / 255, (float)LDcolor.b / 255, 1);
        Vector4 RUcolorf((float)RUcolor.r / 255, (float)RUcolor.g / 255, (float)RUcolor.b / 255, 1);
        Vector4 RDcolorf((float)RDcolor.r / 255, (float)RDcolor.g / 255, (float)RDcolor.b / 255, 1);
        Vector4 Ucolorf, Dcolorf, CenterColor;
        Ucolorf = LUcolorf*djR + RUcolorf*djL;
        Dcolorf = LDcolorf*djR + RDcolorf*djL;
        CenterColor = Ucolorf*diD + Dcolorf*diU;
        color = CenterColor;

        return color;
    }

}; // end of namespace KGE
