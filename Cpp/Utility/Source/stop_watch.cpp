// Simple wall clock stop watch class.
#include "Utility/stop_watch.h"

#include "Utility/exception_handling.h"
#include "Utility/stop_watch.h"

#include <ratio>

namespace Utility {
StopWatch::StopWatch() {
  startTimePoint = std::chrono::steady_clock::now();
}

void StopWatch::stop() {
  throwExceptionIfFalse(isActive, "Only an active stop watch can be stopped.");
  endTimePoint = std::chrono::steady_clock::now();
  isActive = false;
}

double StopWatch::getElapsedTimeInSeconds() const {
  if (isActive) {
    return std::chrono::duration<double>(std::chrono::steady_clock::now()
                                         - startTimePoint)
        .count();
  } else {
    return std::chrono::duration<double>(endTimePoint - startTimePoint).count();
  }
}
}  // namespace Utility
