#pragma once
#include "../../framework.h"
#include <string.h>
#include <string>

struct MColor
{
	BYTE R;
	BYTE G;
	BYTE B;
	MColor()
		: R(0), G(0), B(0)
	{

	}
	MColor(BYTE r, BYTE g, BYTE b)
		: R(r), G(g), B(b)
	{

	}

	std::string ToString()
	{
		return "(" + std::to_string(R) + ", " + std::to_string(G) + ", " +
			std::to_string(B) + ")";
	}

} typedef MColor;

extern MColor Black;
extern MColor White;