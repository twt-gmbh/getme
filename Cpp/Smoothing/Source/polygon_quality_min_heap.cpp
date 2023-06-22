// Penalty corrected polygonal element quality min heap used for
// selecting the polygon to transform in GETMe sequential.
#include "polygon_quality_min_heap.h"

#include "Mesh/polygonal_mesh.h"
#include "Mesh/polygonal_mesh_algorithms.h"
#include "Utility/exception_handling.h"

#include <algorithm>
#include <cstddef>

namespace Smoothing {
PolygonQualityMinHeap::PolygonQualityMinHeap(const Mesh::PolygonalMesh& mesh)
  : polygonIndexToBinaryTreeEntryIndex(
      mesh.getNumberOfPolygons(),
      std::numeric_limits<std::size_t>::max()) {
  binaryTree.reserve(mesh.getNumberOfPolygons());

  const auto meanRatioQualityNumbers =
      Mesh::computeMeanRatioQualityNumberOfPolygons(mesh.getPolygons(),
                                                    mesh.getNodes());
  for (std::size_t polygonIndex = 0; polygonIndex < mesh.getNumberOfPolygons();
       ++polygonIndex) {
    binaryTree.emplace_back(polygonIndex,
                            meanRatioQualityNumbers.at(polygonIndex),
                            mesh.isFixedPolygon(polygonIndex));
    polygonIndexToBinaryTreeEntryIndex.at(polygonIndex) = polygonIndex;
    minHeapifyEntryOfPolygon(polygonIndex);
  }
}

inline bool PolygonQualityMinHeap::isFirstQualityLower(
    const std::size_t firstEntryIndex,
    const std::size_t secondEntryIndex) const {
  return binaryTree.at(firstEntryIndex) < binaryTree.at(secondEntryIndex);
}

void PolygonQualityMinHeap::swapMinHeapEntriesAndAdjustMapping(
    const std::size_t firstEntryIndex,
    const std::size_t secondEntryIndex) {
  std::swap(binaryTree.at(firstEntryIndex), binaryTree.at(secondEntryIndex));
  polygonIndexToBinaryTreeEntryIndex.at(
      binaryTree.at(firstEntryIndex).getPolygonIndex()) = firstEntryIndex;
  polygonIndexToBinaryTreeEntryIndex.at(
      binaryTree.at(secondEntryIndex).getPolygonIndex()) = secondEntryIndex;
}

void PolygonQualityMinHeap::minHeapifyEntryOfPolygon(
    const std::size_t polygonIndex) {
  size_t entryIndex = polygonIndexToBinaryTreeEntryIndex.at(polygonIndex);

  // Up-swaps
  while (entryIndex > 0) {
    const std::size_t parentEntryIndex = (entryIndex - 1) / 2;
    if (isFirstQualityLower(parentEntryIndex, entryIndex)) {
      // Correct order - terminate.
      break;
    }
    swapMinHeapEntriesAndAdjustMapping(parentEntryIndex, entryIndex);
    entryIndex = parentEntryIndex;
  }

  // Down-swaps
  const std::size_t maxEntryIndex = binaryTree.size();
  while (entryIndex < maxEntryIndex) {
    std::size_t leftChildIndex = 2 * entryIndex + 1;
    std::size_t rightChildIndex = leftChildIndex + 1;
    std::size_t doCompareWithLeftChild = leftChildIndex < maxEntryIndex;
    std::size_t doCompareWithRightChild = rightChildIndex < maxEntryIndex;

    // Start comparing with lowest quality child.
    if (doCompareWithLeftChild && doCompareWithRightChild
        && isFirstQualityLower(rightChildIndex, leftChildIndex)) {
      std::swap(leftChildIndex, rightChildIndex);
      std::swap(doCompareWithLeftChild, doCompareWithRightChild);
    }

    if (doCompareWithLeftChild
        && isFirstQualityLower(leftChildIndex, entryIndex)) {
      swapMinHeapEntriesAndAdjustMapping(leftChildIndex, entryIndex);
      entryIndex = leftChildIndex;
    } else if (doCompareWithRightChild
               && isFirstQualityLower(rightChildIndex, entryIndex)) {
      swapMinHeapEntriesAndAdjustMapping(rightChildIndex, entryIndex);
      entryIndex = rightChildIndex;
    } else {
      // Polygon quality is smaller than both children.
      break;
    }
  }
}

bool PolygonQualityMinHeap::isConsistent() const {
  const bool doSizesMatch =
      binaryTree.size() == polygonIndexToBinaryTreeEntryIndex.size();
  return doSizesMatch && isPolygonIndexToBinaryTreeEntryIndexConsistent()
         && isBinaryTreeConsistent();
}

double PolygonQualityMinHeap::getQMinStar() const {
  // Since the min heap is ordered by penalty corrected
  // polygon quality, we have to determine the minimal
  // polygon quality by iterating over non fixed polygons.
  if (isAllFixedMesh()) {
    Utility::throwException(
        "QMinStar is not defined for all fixed polygon meshes.");
  }
  double qMinStar = std::numeric_limits<double>::infinity();
  for (const auto& entry : binaryTree) {
    if (!entry.isAllFixedNodesPolygon()
        && entry.getMeanRatioNumber() < qMinStar) {
      qMinStar = entry.getMeanRatioNumber();
    }
  }
  return qMinStar;
}

bool PolygonQualityMinHeap::containsAnInvalidPolygon() const {
  const auto isInvalidPolygonPredicate = [](const MinHeapEntry& entry) {
    return entry.getMeanRatioNumber() < 0.0;
  };
  return std::ranges::any_of(binaryTree, isInvalidPolygonPredicate);
}

bool PolygonQualityMinHeap::isPolygonIndexToBinaryTreeEntryIndexConsistent()
    const {
  // Check if indices are consistent.
  for (std::size_t polygonIndex = 0;
       polygonIndex < polygonIndexToBinaryTreeEntryIndex.size();
       ++polygonIndex) {
    const auto indexInBinaryTree =
        polygonIndexToBinaryTreeEntryIndex.at(polygonIndex);
    if (polygonIndex != binaryTree.at(indexInBinaryTree).getPolygonIndex()) {
      return false;
    }
  }
  // Check if all binary tree entries are indexed.
  auto indexedBinaryTreePolygons = polygonIndexToBinaryTreeEntryIndex;
  std::ranges::sort(indexedBinaryTreePolygons);
  for (std::size_t index = 0; index < polygonIndexToBinaryTreeEntryIndex.size();
       ++index) {
    if (index != indexedBinaryTreePolygons.at(index))
      return false;
  }
  return true;
}

bool PolygonQualityMinHeap::isBinaryTreeConsistent() const {
  const std::size_t numberOfEntries = binaryTree.size();
  for (std::size_t entryIndex = 0; entryIndex < numberOfEntries; entryIndex++) {
    const std::size_t leftChildIndex = 2 * entryIndex + 1;
    if ((leftChildIndex < numberOfEntries)
        && isFirstQualityLower(leftChildIndex, entryIndex)) {
      return false;
    }
    const std::size_t rightChildIndex = leftChildIndex + 1;
    if ((rightChildIndex < numberOfEntries)
        && isFirstQualityLower(rightChildIndex, entryIndex)) {
      return false;
    }
  }
  return true;
}
}  // namespace Smoothing
