#include "utils.h"
#include <chrono>
#include <cstdint>
#include <thread>

namespace utils {

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

TickClock::TickClock(int sleepTimeMS): _sleepTimeMS(sleepTimeMS) {
    this->_epochSyncMS = timeSinceEpochMillisec();
}

bool TickClock::synchronize() {
    uint64_t timeSinceLastTickMS = timeSinceEpochMillisec() - this->_epochSyncMS;
    std::this_thread::sleep_for(std::chrono::milliseconds(this->_sleepTimeMS - timeSinceLastTickMS));

    this->_epochSyncMS = timeSinceEpochMillisec();
    return (timeSinceLastTickMS >= 0);
}

}  // namespace utils
