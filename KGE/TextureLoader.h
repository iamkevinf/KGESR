#ifndef __TEXTURE_LOADER_H__
#define __TEXTURE_LOADER_H__

#include "KGEMath.h"

#include <string>

class ColorRGB
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;

    ColorRGB(unsigned char _r, unsigned char _g, unsigned char _b) :
        r(_r),
        g(_g),
        b(_b)
    {

    }
};

class TextureLoader
{
public:
    TextureLoader();
    ~TextureLoader();

    bool ReadBmp(const std::string & filename);
    void Draw(HDC hdc);
    ColorRGB GetPixelColor(const Vector2 & uv);

public:
    unsigned char * buff;
    int width;
    int height;
    RGBQUAD * colorTable;
    int count;
    int lineByte;
};

#endif // __TEXTURE_LOADER_H__

