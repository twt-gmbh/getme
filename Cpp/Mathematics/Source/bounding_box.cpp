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
#include "Mathematics/bounding_box.h"

#include "Utility/exception_handling.h"

namespace Mathematics {
BoundingBox::BoundingBox(const double xMin,
                         const double xMax,
                         const double yMin,
                         const double yMax)
  : xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax) {
  Utility::throwExceptionIfFalse(xMin <= xMax, "xMin <= xMax expected.");
  Utility::throwExceptionIfFalse(yMin <= yMax, "yMin <= yMax expected.");
}
}  // namespace Mathematics
