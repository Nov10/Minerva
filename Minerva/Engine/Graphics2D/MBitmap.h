#pragma once

#include "MColor.h"
#include <functional>

class MBitmap
{
public:
	MBitmap(int width, int height);
	MBitmap() = delete;
	~MBitmap();
	MBitmap& operator=(const MBitmap&) = delete; // 복사 대입 금지
	void CopyFrom(const MBitmap& bitmap);
	inline MColor Get(int x, int y) const;
	inline void Set(int x, int y, MColor color);
	inline int GetWidth() const { return Width; }
	inline int GetHeight() const { return Height; }
	HBITMAP ToHBITMAP(HDC hdc) const;
	void Clear(MColor color);
	void ClearBlack();
	void ClearWhite();	
	void Release();
private:
	void SetData(MColor* colors);
	int GetIndex(int x, int y) const;
	void Loop(std::function<void(int, int)> func);
private:
	int Width;
	int Height;
	MColor* Colors;
};

