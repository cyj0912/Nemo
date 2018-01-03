#include "Timeline.h"

#if TC_OS == TC_OS_WINDOWS_NT
#include <Windows.h>
#endif

namespace tc
{

void FHighResolutionClock::Init()
{
    LARGE_INTEGER li;

    bPause = false;

    QueryPerformanceFrequency(&li);
    Frequency = (double)li.QuadPart;

    QueryPerformanceCounter(&li);
    CountStart = li.QuadPart;
}

uint64_t FHighResolutionClock::Now() const
{
    if (bPause)
        return CountWhenPaused;

    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return li.QuadPart - CountStart;
}

int FHighResolutionClock::NowMilliSec() const
{
    return Now() * 1000 / Frequency;
}

double FHighResolutionClock::GetFrequency() const
{
    return Frequency;
}

void FHighResolutionClock::Pause()
{
    CountWhenPaused = Now();
    bPause = true;
}

void FHighResolutionClock::UnPause()
{
    CountStart += Now() - CountWhenPaused;
    bPause = false;
}

bool FHighResolutionClock::IsPaused() const
{
    return bPause;
}

}
