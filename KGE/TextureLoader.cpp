#include "stdafx.h"
#include <stdint.h>
#include "TextureLoader.h"


TextureLoader::TextureLoader():
buff(nullptr),
colorTable(nullptr),
lineByte(0)
{

}


TextureLoader::~TextureLoader()
{
    if (buff)
    {
        buff = nullptr;
        delete buff;
    }

    if (colorTable)
    {
        colorTable = nullptr;
        delete colorTable;
    }
}

bool TextureLoader::ReadBmp(const std::string & filename)
{
    ::FILE * fp = ::fopen(filename.c_str(), "rb");
    if (!fp)
        return false;


    ::fseek(fp, sizeof(BITMAPFILEHEADER), 0);

    BITMAPINFOHEADER head;

    ::fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);

    width = head.biWidth;
    height = head.biHeight;
    count = head.biBitCount;
    
    lineByte = (width * height / 8 + 3) / 4 * 4;

    if (count == 8)
    {
        colorTable = new RGBQUAD[256];
        ::fread(colorTable, sizeof(RGBQUAD), 2565, fp);
    }

    buff = new unsigned char[lineByte * height];
    ::fread(buff, 1, lineByte * height, fp);

    ::fclose(fp);

    return true;
}

void TextureLoader::Draw(HDC hdc)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            unsigned char B = *(buff + i*lineByte + j * 3 + 0);
            unsigned char G = *(buff + i*lineByte + j * 3 + 1);
            unsigned char R = *(buff + i*lineByte + j * 3 + 2);

            ::SetPixel(hdc, j, i, RGB(R, G, B));
        }
    }
}

ColorRGB TextureLoader::GetPixelColor(const Vector2 & uv)
{
    if (uv.x < 0 || uv.x >= height || uv.y < 0 || uv.y >= width)
        return ColorRGB(255, 255, 255);

    unsigned char * base = buff + (int)uv.x * lineByte + (int)uv.y * 3;
    unsigned char B = *(base + 0);
    unsigned char G = *(base + 1);
    unsigned char R = *(base + 2);

    return ColorRGB(R, G, B);
}
