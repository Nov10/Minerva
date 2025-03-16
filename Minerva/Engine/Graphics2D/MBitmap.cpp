#include "MBitmap.h"
#include <algorithm>

MBitmap::MBitmap(int width, int height) : Width(width), Height(height)
{
	Colors = new MColor[width * height];
}

MBitmap::~MBitmap()
{
    delete[] Colors;
}

void MBitmap::SetData(MColor* colors)
{
    memcpy(Colors, colors, GetWidth() * GetHeight());
}

int MBitmap::GetIndex(int x, int y) const {
	return x + ((Height - 1 - y) * Width);
}

void MBitmap::Loop(std::function<void(int, int)> func)
{
    for (int x = 0; x < GetWidth(); x++)
    {
        for (int y = 0; y < GetHeight(); y++)
        {
            func(x, y);
        }
    }
}

void MBitmap::CopyFrom(const MBitmap& bitmap)
{
    Release();
    Width = bitmap.Width;
    Height = bitmap.Height;
    Colors = new MColor[Width * Height];
    std::copy(bitmap.Colors, bitmap.Colors + (GetWidth() * GetHeight()), Colors);
}

inline MColor MBitmap::Get(int x, int y) const
{
	return Colors[GetIndex(x, y)];
}
inline void MBitmap::Set(int x, int y, MColor color)
{
	Colors[GetIndex(x, y)] = color;
}

HBITMAP MBitmap::ToHBITMAP(HDC hdc) const
{
    // 비트맵 정보 생성
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = Width;
    bmi.bmiHeader.biHeight = -Height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* pBits = nullptr;
    HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);

    if (!hBitmap || !pBits)
        return nullptr;

    auto* pixels = static_cast<BYTE*>(pBits);
    for (int y = 0; y < Height; ++y)
    {
        for (int x = 0; x < Width; ++x)
        {
            MColor color = Get(x, y);
            int index = GetIndex(x, y) * 4;
            pixels[index + 0] = color.B; // Blue
            pixels[index + 1] = color.G; // Green
            pixels[index + 2] = color.R; // Red
            pixels[index + 3] = 255;     // Alpha (Opaque)
        }
    }

    return hBitmap;
}

void MBitmap::Clear(MColor color)
{
    Loop([this, color](int x, int y) -> void
    {
            this->Set(x, y, color);
    });
}

void MBitmap::ClearBlack()
{
    Clear(Black);
}

void MBitmap::ClearWhite()
{
    Clear(White);
}

void MBitmap::Release()
{
    Width = 0;
    Height = 0;
    delete[] Colors;
}
