#pragma once
#include <windows.h>
#include <iostream>

class Timer {
private:
    static LARGE_INTEGER frequency;
    static LARGE_INTEGER prevTime;
    static LARGE_INTEGER currentTime;
    static double deltaTime;
    static double totalTime;

public:
    static void Initialize() {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&prevTime);
        deltaTime = 0.0;
        totalTime = 0.0;
    }

    static void Update() {
        QueryPerformanceCounter(&currentTime);
        deltaTime = static_cast<double>(currentTime.QuadPart - prevTime.QuadPart) / frequency.QuadPart;
        totalTime += deltaTime;
        prevTime = currentTime;
    }

    static float GetDeltaTime() {
        return deltaTime;
    }

    static float GetTotalTime() {
        return totalTime;
    }
};
