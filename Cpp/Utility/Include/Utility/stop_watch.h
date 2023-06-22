// Simple wall clock stop watch class.
#pragma once

#include <chrono>

namespace Utility {
class StopWatch {
public:
  // Stop watch ist started by constructor.
  StopWatch();

  void stop();

  double getElapsedTimeInSeconds() const;

private:
  bool isActive = true;
  std::chrono::time_point<std::chrono::steady_clock> startTimePoint;
  std::chrono::time_point<std::chrono::steady_clock> endTimePoint;
};
}  // namespace Utility
