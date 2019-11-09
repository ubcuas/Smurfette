#pragma once

#include <cstdint>

namespace utils {

uint64_t timeSinceEpochMillisec();

class TickClock {
public:
    TickClock(int sleepTimeMS);
    bool synchronize();

private:
    int _sleepTimeMS;
    uint64_t _epochSyncMS;
};

} // namespace utils