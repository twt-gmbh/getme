/*
Representation of a planar polygonal element with at least three nodes as

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
// sequence of pairwise different zero based node indices.
#include "Mathematics/polygon.h"

#include "Utility/exception_handling.h"

#include <unordered_set>

namespace Mathematics {
Polygon::Polygon(const std::vector<std::size_t>& nodeIndices)
  : nodeIndices(nodeIndices) {
  Utility::throwExceptionIfFalse(nodeIndices.size() >= 3,
                                 "Polygon must consist of at least 3 nodes.");
  const std::unordered_set<std::size_t> uniqueNodeIndices(nodeIndices.begin(),
                                                          nodeIndices.end());
  Utility::throwExceptionIfFalse(
      nodeIndices.size() == uniqueNodeIndices.size(),
      "Duplicate node indices are not allowed in polygonal elements.");
}
}  // namespace Mathematics
