/*
Representation of an axis aligned bounding box.

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

namespace Mathematics {
class BoundingBox final {
public:
  BoundingBox(const double xMin,
              const double xMax,
              const double yMin,
              const double yMax);

  bool operator==(const BoundingBox& other) const = default;

  double getXMin() const { return xMin; }

  double getXMax() const { return xMax; }

  double getYMin() const { return yMin; }

  double getYMax() const { return yMax; }

  double getXDimension() const { return xMax - xMin; }

  double getYDimension() const { return yMax - yMin; }

private:
  const double xMin;
  const double xMax;
  const double yMin;
  const double yMax;
};
}  // namespace Mathematics
