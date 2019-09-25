#include "utils.h"
#include <chrono>
#include <cstdint>

namespace utils {

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

}  // namespace utils
