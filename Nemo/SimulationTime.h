#pragma once

#include <cstdint>

namespace tc
{

/**
 * Stores time in milliseconds
 */
class FSimulationTime
{
public:
    void AdvanceTime(uint32_t milliSec);

    uint64_t GetCurrentTime() const;

    uint32_t GetDeltaTime() const;

private:
    uint64_t CurrentTime = 0;
    uint64_t LastTime = 0;
};

} /* namespace tc */
