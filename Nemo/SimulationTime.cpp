#include "SimulationTime.h"

namespace tc
{

void FSimulationTime::AdvanceTime(uint32_t milliSec)
{
    LastTime = CurrentTime;
    CurrentTime += milliSec;
}

uint64_t FSimulationTime::GetCurrentTime() const
{
    return CurrentTime;
}

uint32_t FSimulationTime::GetDeltaTime() const
{
    return static_cast<uint32_t>(CurrentTime - LastTime);
}

} /* namespace tc */
