#pragma once
#include "FoundationAPI.h"
#include "Platform.h"
#include <cstdint>

namespace tc
{

class FOUNDATION_API FHighResolutionClock
{
    uint64_t CountStart;
    uint64_t CountWhenPaused;
    double Frequency;

    bool bPause;

public:
    void Init();

    uint64_t Now() const;
    int NowMilliSec() const;
    double GetFrequency() const;

    void Pause();
    void UnPause();
    bool IsPaused() const;
};

}
