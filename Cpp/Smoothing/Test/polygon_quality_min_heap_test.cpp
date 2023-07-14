/*
Unit tests for the penalty corrected polygon quality min heap.

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
#include "../Source/polygon_quality_min_heap.h"

#include "Mesh/polygonal_mesh.h"
#include "Testdata/meshes.h"

#include "gtest/gtest.h"

#include <vector>

TEST(MinHeapEntry, constructorAndGetters_initialPenaltySumDefault) {
  const std::size_t polygonIndex = 17;
  const double polygonMeanRatioNumber = 0.74;
  const bool isFixedPolygon = true;
  const Smoothing::MinHeapEntry entry(polygonIndex, polygonMeanRatioNumber,
                                      isFixedPolygon);

  EXPECT_EQ(polygonIndex, entry.getPolygonIndex());
  EXPECT_EQ(polygonMeanRatioNumber, entry.getPenaltyCorrectedMeanRatioNumber());
  EXPECT_EQ(polygonMeanRatioNumber, entry.getMeanRatioNumber());
  EXPECT_EQ(isFixedPolygon, entry.isAllFixedNodesPolygon());
}

TEST(MinHeapEntry, constructorAndGetters_initialPenaltySumGiven) {
  const std::size_t polygonIndex = 3;
  const double polygonMeanRatioNumber = 0.4;
  const bool isFixedPolygon = false;
  const double initialPenaltySum = 0.06;
  const Smoothing::MinHeapEntry entry(polygonIndex, polygonMeanRatioNumber,
                                      isFixedPolygon, initialPenaltySum);

  EXPECT_EQ(polygonIndex, entry.getPolygonIndex());
  EXPECT_EQ(polygonMeanRatioNumber + initialPenaltySum,
            entry.getPenaltyCorrectedMeanRatioNumber());
  EXPECT_EQ(polygonMeanRatioNumber, entry.getMeanRatioNumber());
  EXPECT_EQ(isFixedPolygon, entry.isAllFixedNodesPolygon());
}

TEST(MinHeapEntry,
     constructorAndGetters_initialPenaltySumGreaterEqualZeroEnforced) {
  const std::size_t polygonIndex = 3;
  const double polygonMeanRatioNumber = 0.4;
  const bool isFixedPolygon = false;
  const double initialPenaltySum = -0.4;
  const Smoothing::MinHeapEntry entry(polygonIndex, polygonMeanRatioNumber,
                                      isFixedPolygon, initialPenaltySum);

  EXPECT_EQ(polygonIndex, entry.getPolygonIndex());
  EXPECT_EQ(polygonMeanRatioNumber, entry.getPenaltyCorrectedMeanRatioNumber());
  EXPECT_EQ(polygonMeanRatioNumber, entry.getMeanRatioNumber());
  EXPECT_EQ(isFixedPolygon, entry.isAllFixedNodesPolygon());
}

TEST(MinHeapEntry, compareOperators) {
  // Test polygons in expected ascending order.
  // Constructor data: polygon index, mean ration, isFixed, initial penalty sum.
  const std::vector<Smoothing::MinHeapEntry> orderedEntries{
      {14, 0.2, false, 0.0},    // q sum = 0.2
      {30, 0.18, false, 0.04},  // q sum = 0.22, small q(E)
      {9, 0.2, false, 0.02},    // q sum = 0.22, low polygon index
      {11, 0.2, false, 0.02},   // q sum = 0.22, larger polygon index
      {2, 0.22, false, 0.00},   // q sum = 0.22, large q(E)
      {4, 0.1, false, 0.13},    // q sum = 0.23, sum larger
      {1, 0.1, true, 0.0},      // fixed is always larger than non fixed
      {0, 0.2, true, 0.0},      // fixed are also sorted by subsequent criteria
  };

  for (std::size_t firstIndex = 0; firstIndex < orderedEntries.size();
       ++firstIndex) {
    for (std::size_t secondIndex = 0; secondIndex < orderedEntries.size();
         ++secondIndex) {
      const auto& firstEntry = orderedEntries.at(firstIndex);
      const auto& secondEntry = orderedEntries.at(secondIndex);

      EXPECT_EQ(firstIndex < secondIndex, firstEntry < secondEntry);
      EXPECT_EQ(firstIndex == secondIndex, firstEntry == secondEntry);
      EXPECT_EQ(firstIndex > secondIndex, firstEntry > secondEntry);
    }
  }
}

TEST(MinHeapEntry, updateMethods_throwIfPolygonIsFixed) {
  const std::size_t polygonIndex = 1;
  const double polygonMeanRatioNumber = 0.3;
  const double updatedPolygonMeanRatioNumber = 0.37;
  Smoothing::MinHeapEntry entry(polygonIndex, polygonMeanRatioNumber, true);
  EXPECT_ANY_THROW(entry.updateMeanRatioNumber(updatedPolygonMeanRatioNumber));
  double penaltyChange = 0.3;
  EXPECT_ANY_THROW(entry.updateMeanRatioNumberAndAddToPenaltySum(
      updatedPolygonMeanRatioNumber, penaltyChange));
}

TEST(MinHeapEntry, updateMethods) {
  const std::size_t polygonIndex = 12;
  const double polygonMeanRatioNumber = 0.33;
  Smoothing::MinHeapEntry entry(polygonIndex, polygonMeanRatioNumber, false);

  const double firstPolygonMeanRatioNumberUpdate = 0.4;
  entry.updateMeanRatioNumber(firstPolygonMeanRatioNumberUpdate);
  EXPECT_EQ(firstPolygonMeanRatioNumberUpdate, entry.getMeanRatioNumber());
  EXPECT_EQ(firstPolygonMeanRatioNumberUpdate,
            entry.getPenaltyCorrectedMeanRatioNumber());

  const double secondPolygonMeanRatioNumberUpdate = 0.6;
  const double penaltyChange = 0.02;
  entry.updateMeanRatioNumberAndAddToPenaltySum(
      secondPolygonMeanRatioNumberUpdate, penaltyChange);
  EXPECT_EQ(secondPolygonMeanRatioNumberUpdate, entry.getMeanRatioNumber());
  EXPECT_EQ(secondPolygonMeanRatioNumberUpdate + penaltyChange,
            entry.getPenaltyCorrectedMeanRatioNumber());

  // Penalty sum is >= 0.0 check.
  const double secondPenaltyChange = -1.0;
  entry.updateMeanRatioNumberAndAddToPenaltySum(
      firstPolygonMeanRatioNumberUpdate, secondPenaltyChange);
  EXPECT_EQ(firstPolygonMeanRatioNumberUpdate, entry.getMeanRatioNumber());
  EXPECT_EQ(firstPolygonMeanRatioNumberUpdate,
            entry.getPenaltyCorrectedMeanRatioNumber());
}

TEST(MinHeapEntry, addToPenaltySum) {
  const std::size_t polygonIndex = 12;
  const double polygonMeanRatioNumber = 0.33;
  Smoothing::MinHeapEntry entry(polygonIndex, polygonMeanRatioNumber, false);

  const double penaltyChange = 0.4;
  entry.addToPenaltySum(penaltyChange);
  EXPECT_EQ(polygonMeanRatioNumber + penaltyChange,
            entry.getPenaltyCorrectedMeanRatioNumber());

  // Penalty sum is >= 0.0 check.
  entry.addToPenaltySum(-2.0 * penaltyChange);
  EXPECT_EQ(polygonMeanRatioNumber, entry.getPenaltyCorrectedMeanRatioNumber());
}

/* Information: Initial mesh polygon index and mean ratio
  {0, 6.298366572977736e-01},
  {4, 7.085662394599952e-01},
  {5, 7.804455606313192e-01},
  {2, 9.141634485276990e-01},
  {6, 9.742785792574934e-01},
  {1, 9.897433186107867e-01},
  {3, 1.000000000000000e+00},
*/

TEST(PolygonQualityMinHeap, constructorAndLowestQualityPolygonIndexGetter) {
  const auto mesh = Testdata::getMixedSampleMesh();
  const Smoothing::PolygonQualityMinHeap minHeap(mesh);
  EXPECT_NO_THROW(minHeap.isConsistent());
  EXPECT_EQ(0, minHeap.getLowestQualityPolygonIndex());

  EXPECT_TRUE(minHeap.isConsistent());
}

namespace {
struct TestData {
  std::size_t indexOfPolygonToAdjust;
  double newPolygonMeanRatioNumber;
  double penaltyChange;
  std::size_t expectedNewWorstPolygonIndex;
};
}  // namespace

TEST(PolygonQualityMinHeap, updateTests) {
  const auto mesh = Testdata::getMixedSampleMesh();
  const std::vector<TestData> testCases{
      {0, 0.6, 0.0, 0},   // Polygon 0 is still worst after update.
      {0, 0.6, 0.1, 0},   // Polygon 0 is still worst after update.
      {0, 0.71, 0.0, 4},  // Polygon 0 is second worst.
      {0, 1.0, 0.1, 4},   // Polygon 0 is best.
      {5, 0.6, 0.0, 5},   // Make polygon 5 worst.
      {3, 0.6, 0.0, 3},   // Make polygon 3 worst.
  };

  for (const auto& testData : testCases) {
    Smoothing::PolygonQualityMinHeap minHeap(mesh);
    if (testData.penaltyChange == 0.0) {
      minHeap.updateMeanRatioNumberIfNotFixedPolygon(
          testData.indexOfPolygonToAdjust, testData.newPolygonMeanRatioNumber);
    } else {
      minHeap.updateMeanRatioNumberAndAddToPenaltySum(
          testData.indexOfPolygonToAdjust, testData.newPolygonMeanRatioNumber,
          testData.penaltyChange);
    }
    const auto newWorstPolygonIndex = minHeap.getLowestQualityPolygonIndex();
    EXPECT_EQ(testData.expectedNewWorstPolygonIndex, newWorstPolygonIndex);
    EXPECT_TRUE(minHeap.isConsistent());
  }
}

TEST(PolygonQualityMinHeap, addToPenaltySum) {
  const auto mesh = Testdata::getMixedSampleMesh();
  Smoothing::PolygonQualityMinHeap minHeap(mesh);
  minHeap.addToPenaltySum(0, 0.1);  // Make polygon 0 second worst.
  EXPECT_EQ(4, minHeap.getLowestQualityPolygonIndex());

  minHeap.addToPenaltySum(4, 0.1);  // Make polygon 0 worst again.
  EXPECT_EQ(0, minHeap.getLowestQualityPolygonIndex());

  minHeap.addToPenaltySum(0, 0.2);  // Polygon 5 becomes worst.
  EXPECT_EQ(5, minHeap.getLowestQualityPolygonIndex());

  EXPECT_TRUE(minHeap.isConsistent());
}

TEST(PolygonQualityMinHeap, isAllFixedMesh_notAllFixedNodes) {
  const auto mesh = Testdata::getMixedSampleMesh();
  Smoothing::PolygonQualityMinHeap minHeap(mesh);
  EXPECT_FALSE(minHeap.isAllFixedMesh());
}

TEST(PolygonQualityMinHeap, isAllFixedMesh_allFixedNodes) {
  const auto nodes = Testdata::getMixedSampleMeshNodes();
  const auto polygons = Testdata::getMixedSampleMeshPolygons();
  auto fixedNodeIndices = Testdata::getMixedSampleMeshFixedNodeIndices();
  fixedNodeIndices.insert(9);
  fixedNodeIndices.insert(10);
  const Mesh::PolygonalMesh mesh(nodes, polygons, fixedNodeIndices);
  Smoothing::PolygonQualityMinHeap minHeap(mesh);
  EXPECT_TRUE(minHeap.isAllFixedMesh());
}

TEST(PolygonQualityMinHeap, getQMinStar_equalsQMin) {
  const auto mesh = Testdata::getMixedSampleMesh();
  Smoothing::PolygonQualityMinHeap minHeap(mesh);
  const double tolerance = 1.0e-15;
  // Lowest quality polygon has index zero with q_min = q_min*.
  EXPECT_NEAR(6.298366572977736e-01, minHeap.getQMinStar(), tolerance);
}

TEST(PolygonQualityMinHeap, getQMinStar_differsFromQMin) {
  const auto nodes = Testdata::getMixedSampleMeshNodes();
  // Make polygon with index zero all fixed making polygon with index 4 the
  // worst. Thus q_min and q_min* differ.
  const auto polygons = Testdata::getMixedSampleMeshPolygons();
  auto fixedNodeIndices = Testdata::getMixedSampleMeshFixedNodeIndices();
  fixedNodeIndices.insert(10);
  const Mesh::PolygonalMesh mesh(nodes, polygons, fixedNodeIndices);
  Smoothing::PolygonQualityMinHeap minHeap(mesh);
  const double tolerance = 1.0e-15;
  EXPECT_NEAR(7.085662394599952e-01, minHeap.getQMinStar(), tolerance);
}

TEST(PolygonQualityMinHeap, getQMinStar_throwIfAllFixedMesh) {
  const auto nodes = Testdata::getMixedSampleMeshNodes();
  const auto polygons = Testdata::getMixedSampleMeshPolygons();
  auto fixedNodeIndices = Testdata::getMixedSampleMeshFixedNodeIndices();
  fixedNodeIndices.insert(9);
  fixedNodeIndices.insert(10);
  const Mesh::PolygonalMesh fixedMesh(nodes, polygons, fixedNodeIndices);
  Smoothing::PolygonQualityMinHeap minHeap(fixedMesh);
  EXPECT_ANY_THROW(minHeap.getQMinStar());
}

TEST(PolygonQualityMinHeap, containsAnInvalidPolygon) {
  const auto validMesh = Testdata::getMixedSampleMesh();
  Smoothing::PolygonQualityMinHeap validMeshMinHeap(validMesh);
  EXPECT_FALSE(validMeshMinHeap.containsAnInvalidPolygon());

  const auto invalidMesh = Testdata::getInvalidMixedSampleMesh();
  Smoothing::PolygonQualityMinHeap invalidMeshMinHeap(invalidMesh);
  EXPECT_TRUE(invalidMeshMinHeap.containsAnInvalidPolygon());
}
