// Representation of a planar polygonal element with at least three nodes as
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
