// Unit tests for simple wall clock stop watch class.
#include "Utility/stop_watch.h"

#include "gtest/gtest.h"

TEST(StopWatch, measureTime) {
  Utility::StopWatch stopWatch;
  std::this_thread::sleep_for(std::chrono::milliseconds(300));
  const auto elapsedTimeInSecondsBeforeStop =
      stopWatch.getElapsedTimeInSeconds();
  stopWatch.stop();
  const auto elapsedTimeInSecondsAfterStop =
      stopWatch.getElapsedTimeInSeconds();
  const double expectedElapsedTime = 0.3;
  // Comparably large tolerance to consider slow CI systems
  const double tolerance = 0.05;

  EXPECT_NEAR(expectedElapsedTime, elapsedTimeInSecondsBeforeStop, tolerance);
  EXPECT_NEAR(expectedElapsedTime, elapsedTimeInSecondsAfterStop, tolerance);
}

TEST(StopWatch, stop_throwIfAlreadyStopped) {
  Utility::StopWatch stopWatch;
  stopWatch.stop();
  EXPECT_ANY_THROW(stopWatch.stop());
}
