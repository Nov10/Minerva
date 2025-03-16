#pragma once
#include <windows.h>
#include <iostream>
#include  <string>
#include "MMath/MMath.h"

class Debug {
public:

    static void Log(std::string str) {
        std::wstring widestr = std::wstring(str.begin(), str.end());
        const wchar_t* widecstr = widestr.c_str();
        OutputDebugStringW(widecstr);
        OutputDebugStringW(L"\n");
    }
    static void Log(MColor value)
    {
        Debug::Log(value.ToString());
    }
    static void Log(float value)
    {
        Debug::Log(std::to_string(value));
    }
    static void Log(int value)
    {
        Debug::Log(std::to_string(value));
    }
    static void Log(Vector3 value)
    {
        Debug::Log(value.ToString());
    }
    static void Log(Quaternion value)
    {
        Debug::Log(value.ToString());
    }
};
