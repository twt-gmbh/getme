/*
Unit tests for simple wall clock stop watch class.

Copyright (C) 2023  TWT GmbH Science & Innovation.

This file is part of GETMe Polygonal Meshes 2D.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/
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
