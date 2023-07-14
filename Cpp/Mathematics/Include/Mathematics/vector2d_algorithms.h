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
#pragma once

#include "Mathematics/vector2d.h"

#include <vector>

namespace Mathematics {
class Vector2D;
class BoundingBox;

// Check if the given two vectors are equal up to tolerance (Euclidean norm).
inline bool areEqual(const Vector2D& first,
                     const Vector2D& second,
                     const double tolerance) {
  return (first - second).getLengthSquared() <= tolerance * tolerance;
}

// Check if the two given vectors have same length and the entries
// with same index are equal up to tolerance.
bool areEqual(const std::vector<Vector2D>& first,
              const std::vector<Vector2D>& second,
              const double tolerance);

// Get the bounding box of the given points.
BoundingBox getBoundingBox(const std::vector<Vector2D>& points);

// Get random vector with maximal given Euclidean length.
Vector2D getRandomVector(const double maxVectorLength);
}  // namespace Mathematics
