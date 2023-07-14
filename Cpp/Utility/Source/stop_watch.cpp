/*
Simple wall clock stop watch class.

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
