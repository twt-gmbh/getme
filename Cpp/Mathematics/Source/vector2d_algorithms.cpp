/*
Algorithms for 2D vectors.

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
#include "Mathematics/vector2d_algorithms.h"

#include "Mathematics/bounding_box.h"
#include "Utility/exception_handling.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>
#include <random>

bool Mathematics::areEqual(const std::vector<Vector2D>& first,
                           const std::vector<Vector2D>& second,
                           const double tolerance) {
  const auto areEntriesEqual = [tolerance](const Vector2D& firstEntry,
                                           const Vector2D& secondEntry) {
    return areEqual(firstEntry, secondEntry, tolerance);
  };
  return std::ranges::equal(first, second, areEntriesEqual);
}

Mathematics::BoundingBox Mathematics::getBoundingBox(
    const std::vector<Vector2D>& points) {
  Utility::throwExceptionIfTrue(
      points.empty(),
      "Cannot compute bounding box for empty vector of points.");
  double xMin = std::numeric_limits<double>::infinity();
  double xMax = -std::numeric_limits<double>::infinity();
  double yMin = std::numeric_limits<double>::infinity();
  double yMax = -std::numeric_limits<double>::infinity();
  for (const auto& point : points) {
    if (xMin > point.getX()) {
      xMin = point.getX();
    }
    if (xMax < point.getX()) {
      xMax = point.getX();
    }
    if (yMin > point.getY()) {
      yMin = point.getY();
    }
    if (yMax < point.getY()) {
      yMax = point.getY();
    }
  }
  return BoundingBox{xMin, xMax, yMin, yMax};
}

Mathematics::Vector2D Mathematics::getRandomVector(
    const double maxVectorLength) {
  Utility::throwExceptionIfFalse(maxVectorLength > 0.0,
                                 "Radius has to be > 0.0.");
  std::random_device device;
  std::default_random_engine engine(device());
  std::uniform_real_distribution distributionRadius(0.0, maxVectorLength);
  const double radius = distributionRadius(engine);
  std::uniform_real_distribution distributionAngle(0.0, 2.0 * std::numbers::pi);
  const double angle = distributionAngle(engine);
  return Vector2D(radius * std::cos(angle), radius * std::sin(angle));
}
