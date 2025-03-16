#include "Engine/Timer.h"

LARGE_INTEGER Timer::frequency;
LARGE_INTEGER Timer::prevTime;
LARGE_INTEGER Timer::currentTime;
double Timer::deltaTime = 0.0;
double Timer::totalTime = 0.0;