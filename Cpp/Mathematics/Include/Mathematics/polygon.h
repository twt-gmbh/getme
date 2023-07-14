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

// Since we focus on a mesh context, polygons are defined by a sequence of node
// indices. The vector of nodes has to be defined separately.
#pragma once

#include <vector>

namespace Mathematics {
class Polygon final {
public:
  explicit Polygon(const std::vector<std::size_t>& nodeIndices);

  const std::vector<std::size_t>& getNodeIndices() const { return nodeIndices; }

  std::size_t getNumberOfNodes() const { return nodeIndices.size(); }

  std::size_t getNodeIndex(const std::size_t nodeNumber) const {
    return nodeIndices.at(nodeNumber);
  }

  std::size_t getPredecessorNodeIndex(const std::size_t nodeNumber) const {
    return nodeIndices.at(nodeNumber == 0 ? nodeIndices.size() - 1
                                          : nodeNumber - 1);
  }

  std::size_t getSuccessorNodeIndex(const std::size_t nodeNumber) const {
    return nodeIndices.at(
        nodeNumber == nodeIndices.size() - 1 ? 0 : nodeNumber + 1);
  }

  bool operator==(const Polygon& other) const = default;

private:
  std::vector<std::size_t> nodeIndices;
};
}  // namespace Mathematics
