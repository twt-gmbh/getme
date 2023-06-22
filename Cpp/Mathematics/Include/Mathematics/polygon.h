// Representation of a planar polygonal element with at least three nodes as
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
