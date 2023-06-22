// Algorithms for 2D vectors.
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
