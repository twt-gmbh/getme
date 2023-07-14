/*
Representation of a 2D vector or point.

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

#include <cmath>

namespace Mathematics {
class Vector2D final {
public:
  Vector2D(const double x, const double y) : x(x), y(y){};

  double getX() const { return x; }

  double getY() const { return y; }

  Vector2D operator+(const Vector2D& other) const {
    return Vector2D(x + other.x, y + other.y);
  }

  void operator+=(const Vector2D& other) {
    x += other.x;
    y += other.y;
  }

  Vector2D operator-(const Vector2D& other) const {
    return Vector2D(x - other.x, y - other.y);
  }

  void operator/=(const double divisor) {
    x /= divisor;
    y /= divisor;
  }

  bool operator==(const Vector2D& other) const = default;

  inline friend Vector2D operator*(const double factor, const Vector2D& vector);

  inline friend Vector2D operator/(const Vector2D& vector,
                                   const double divisor);

  double getLengthSquared() const { return x * x + y * y; }

  double getLength() const { return std::sqrt(x * x + y * y); }

private:
  double x;
  double y;
};

Vector2D operator*(const double factor, const Vector2D& vector) {
  return Vector2D(factor * vector.x, factor * vector.y);
}

Vector2D operator/(const Vector2D& vector, const double divisor) {
  return Vector2D(vector.x / divisor, vector.y / divisor);
}
}  // namespace Mathematics
