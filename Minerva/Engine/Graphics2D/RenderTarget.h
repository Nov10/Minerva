#pragma once

#include "MBitmap.h"

class RenderTarget
{
public:
	RenderTarget(int width, int height);
	//RenderTarget() = delete;
	RenderTarget& operator=(const RenderTarget& other)
	{
		Bitmap.CopyFrom(other.Bitmap);
		return* this;
	}
	inline int GetWidth() const {
		return Bitmap.GetWidth();
	}
	inline int GetHeight() const {
		return Bitmap.GetHeight();
	}
	void Clear(MColor color) { Bitmap.Clear(color); }
	void Set(const MBitmap& bitmap)
	{
		Bitmap.CopyFrom(bitmap);
	}
	HBITMAP ToHBITMAP(HDC hdc) const { return Bitmap.ToHBITMAP(hdc); }
	MBitmap Bitmap;
private:
};

