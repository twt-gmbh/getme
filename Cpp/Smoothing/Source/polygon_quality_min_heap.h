/*
Penalty corrected polygonal element quality min heap used for

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
// selecting the polygon to transform in GETMe sequential.
#pragma once

#include "Utility/exception_handling.h"

#include <compare>
#include <cstddef>
#include <vector>

namespace Mesh {
class PolygonalMesh;
}  // namespace Mesh

namespace Smoothing {
class MinHeapEntry final {
public:
  MinHeapEntry(const std::size_t polygonIndex,
               const double initialMeanRatioNumber,
               const bool isFixedPolygon,
               const double initialQualityPenaltySum = 0.0)
    : isFixedPolygon(isFixedPolygon)
    , penaltyCorrectedMeanRatioNumber(initialMeanRatioNumber
                                      + std::max(0.0, initialQualityPenaltySum))
    , meanRatioNumber(initialMeanRatioNumber)
    , qualityPenaltySum(initialQualityPenaltySum)
    , polygonIndex(polygonIndex) {}

  // Getter functions

  std::size_t getPolygonIndex() const { return polygonIndex; }

  double getPenaltyCorrectedMeanRatioNumber() const {
    return penaltyCorrectedMeanRatioNumber;
  }

  double getMeanRatioNumber() const { return meanRatioNumber; }

  bool isAllFixedNodesPolygon() const { return isFixedPolygon; }

  // Comparison operators

  constexpr auto operator<=>(const MinHeapEntry& other) const = default;

  // Updating functions

  void updateMeanRatioNumber(const double newPolygonMeanRatioNumber) {
    if (isFixedPolygon) {
      Utility::throwException("All fixed polygon cannot change quality.");
    }
    meanRatioNumber = newPolygonMeanRatioNumber;
    penaltyCorrectedMeanRatioNumber =
        newPolygonMeanRatioNumber + qualityPenaltySum;
  }

  void updateMeanRatioNumberAndAddToPenaltySum(
      const double newPolygonMeanRatioNumber,
      const double penaltyChange) {
    if (isFixedPolygon) {
      Utility::throwException("All fixed polygon cannot change quality.");
    }
    meanRatioNumber = newPolygonMeanRatioNumber;
    qualityPenaltySum = std::max(0.0, qualityPenaltySum + penaltyChange);
    penaltyCorrectedMeanRatioNumber = meanRatioNumber + qualityPenaltySum;
  }

  void addToPenaltySum(const double penaltyChange) {
    qualityPenaltySum = std::max(0.0, qualityPenaltySum + penaltyChange);
    penaltyCorrectedMeanRatioNumber = meanRatioNumber + qualityPenaltySum;
  }

private:
  // Member variables are ordered by sorting priority (defaulted <=> operator).
  bool isFixedPolygon;
  double penaltyCorrectedMeanRatioNumber;
  double meanRatioNumber;
  double qualityPenaltySum;
  std::size_t polygonIndex;
};

// Min heap for lowest penalty corrected polygon quality lookup. Implementation
// uses a binary tree stored in a vector (cf.
// https://en.wikipedia.org/wiki/Binary_heap). To speed up arbitrary polygon
// quality adjustments, a lookup table for polygon index to heap vector index is
// also kept in sync.
class PolygonQualityMinHeap final {
public:
  explicit PolygonQualityMinHeap(const Mesh::PolygonalMesh& mesh);

  std::size_t getLowestQualityPolygonIndex() const {
    return binaryTree.front().getPolygonIndex();
  }

  void updateMeanRatioNumberIfNotFixedPolygon(
      const std::size_t polygonIndex,
      const double newPolygonMeanRatioNumber) {
    auto& entry =
        binaryTree.at(polygonIndexToBinaryTreeEntryIndex.at(polygonIndex));
    if (entry.isAllFixedNodesPolygon()) {
      return;
    }
    entry.updateMeanRatioNumber(newPolygonMeanRatioNumber);
    minHeapifyEntryOfPolygon(polygonIndex);
  }

  void updateMeanRatioNumberAndAddToPenaltySum(
      const std::size_t polygonIndex,
      const double newPolygonMeanRatioNumber,
      const double penaltyChange) {
    binaryTree.at(polygonIndexToBinaryTreeEntryIndex.at(polygonIndex))
        .updateMeanRatioNumberAndAddToPenaltySum(newPolygonMeanRatioNumber,
                                                 penaltyChange);
    minHeapifyEntryOfPolygon(polygonIndex);
  }

  void addToPenaltySum(const std::size_t polygonIndex,
                       const double penaltyChange) {
    binaryTree.at(polygonIndexToBinaryTreeEntryIndex.at(polygonIndex))
        .addToPenaltySum(penaltyChange);
    minHeapifyEntryOfPolygon(polygonIndex);
  }

  // Min heap is expected to be always consistent. Use this consistency check as
  // extra safety layer.
  bool isConsistent() const;

  bool isAllFixedMesh() const {
    // Since fixed polygons are sorted always to the end
    // checking the first polygon of the heap suffices.
    return binaryTree.front().isAllFixedNodesPolygon();
  }

  // Return the lowest improvable polygon quality number.
  double getQMinStar() const;

  bool containsAnInvalidPolygon() const;

private:
  bool isPolygonIndexToBinaryTreeEntryIndexConsistent() const;
  bool isBinaryTreeConsistent() const;

  bool isFirstQualityLower(const std::size_t firstEntryIndex,
                           const std::size_t secondEntryIndex) const;
  void swapMinHeapEntriesAndAdjustMapping(const std::size_t firstEntryIndex,
                                          const std::size_t secondEntryIndex);

  // Correct min heap entry position for the given polygon.
  // To be applied after any modification of the associated min heap entry.
  void minHeapifyEntryOfPolygon(const std::size_t polygonIndex);

  std::vector<MinHeapEntry> binaryTree;
  std::vector<std::size_t> polygonIndexToBinaryTreeEntryIndex;
};
}  // namespace Smoothing
