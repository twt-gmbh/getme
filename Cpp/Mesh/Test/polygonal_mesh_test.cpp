/*
Unit tests for planar polygonal meshes.

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
#include "Mesh/polygonal_mesh.h"

#include "Testdata/meshes.h"

#include "gtest/gtest.h"

TEST(PolygonalMesh, constructorThrowsIfNodeIndexOutOfBound) {
  const auto nodes = Testdata::getMixedSampleMeshNodes();
  auto polygons = Testdata::getMixedSampleMeshPolygons();
  // Manipulate first polygon to have an out of bound node index.
  auto firstPolygonNodeIndices = polygons.front().getNodeIndices();
  firstPolygonNodeIndices.at(0) = nodes.size();
  polygons.front() = Mathematics::Polygon(firstPolygonNodeIndices);

  EXPECT_ANY_THROW(Mesh::PolygonalMesh(nodes, polygons));
}

TEST(PolygonalMesh, gettersForConstructorData) {
  auto mesh = Testdata::getMixedSampleMesh();
  EXPECT_EQ(Testdata::getMixedSampleMeshNodes(), mesh.getNodes());
  EXPECT_EQ(Testdata::getMixedSampleMeshNodes(), mesh.getMutableNodes());
  EXPECT_EQ(Testdata::getMixedSampleMeshPolygons(), mesh.getPolygons());
  EXPECT_EQ(Testdata::getMixedSampleMeshFixedNodeIndices(),
            mesh.getFixedNodeIndices());
}

TEST(PolygonalMesh, setNodes) {
  auto mesh = Testdata::getMixedSampleMesh();
  auto newNodes = Testdata::getMixedSampleMeshNodes();
  newNodes.front() = {111.1, 222.2};
  newNodes.back() = {333.3, 444.4};
  EXPECT_NE(newNodes, mesh.getNodes());
  mesh.setNodes(newNodes);
  EXPECT_EQ(newNodes, mesh.getNodes());
}

TEST(PolygonalMesh, setNodes_throwIfSizeMismatch) {
  auto mesh = Testdata::getMixedSampleMesh();
  auto newNodes = Testdata::getMixedSampleMeshNodes();
  newNodes.emplace_back(111.1, 222.2);
  EXPECT_ANY_THROW(mesh.setNodes(newNodes));
  newNodes.pop_back();
  EXPECT_NO_THROW(mesh.setNodes(newNodes));
  newNodes.pop_back();
  EXPECT_ANY_THROW(mesh.setNodes(newNodes));
}

TEST(PolygonalMesh, numberGetters) {
  const auto mesh = Testdata::getMixedSampleMesh();
  EXPECT_EQ(11, mesh.getNumberOfNodes());
  EXPECT_EQ(7, mesh.getNumberOfPolygons());
  EXPECT_EQ(5, mesh.getMaximalNumberOfPolygonNodes());
}

TEST(PolygonalMesh, getNonFixedNodeIndices) {
  const auto mesh = Testdata::getMixedSampleMesh();
  const std::vector<std::size_t> expectedNonFixedNodeIndices{9, 10};

  EXPECT_EQ(expectedNonFixedNodeIndices, mesh.getNonFixedNodeIndices());
}

TEST(PolygonalMesh, isFixedPolygon) {
  // Additionally fix node 10 in sample mesh. Node 9 remains the only non fixed.
  const auto nodes = Testdata::getMixedSampleMeshNodes();
  const auto polygons = Testdata::getMixedSampleMeshPolygons();
  auto fixedNodeIndices = Testdata::getMixedSampleMeshFixedNodeIndices();
  fixedNodeIndices.insert(10);
  std::unordered_set<std::size_t> expectedFixedPolygons{0, 5, 6};
  const Mesh::PolygonalMesh mesh(nodes, polygons, fixedNodeIndices);

  for (std::size_t polygonIndex = 0; polygonIndex < polygons.size();
       ++polygonIndex) {
    const bool expectedIsPolygonFixed =
        expectedFixedPolygons.contains(polygonIndex);
    EXPECT_EQ(expectedIsPolygonFixed, mesh.isFixedPolygon(polygonIndex));
  }
}

TEST(PolygonalMesh, getIndicesOfEdgeConnectedNodes) {
  auto mixedMesh = Testdata::getMixedSampleMesh();
  std::vector<std::unordered_set<std::size_t>>
      expectedIndicesOfEdgeConnectedNodes{
          {1, 10, 8},      // 0
          {0, 10, 9, 2},   // 1
          {1, 3},          // 2
          {2, 4},          // 3
          {3, 9, 5},       // 4
          {6, 4},          // 5
          {5, 9, 10, 7},   // 6
          {6, 8},          // 7
          {7, 10, 0},      // 8
          {1, 4, 6, 10},   // 9
          {1, 9, 6, 8, 0}  // 10
      };
  for (std::size_t nodeIndex = 0; nodeIndex < mixedMesh.getNumberOfNodes();
       ++nodeIndex) {
    EXPECT_EQ(expectedIndicesOfEdgeConnectedNodes.at(nodeIndex),
              mixedMesh.getIndicesOfEdgeConnectedNodes(nodeIndex));
  }
}

TEST(PolygonalMesh, getAttachedPolygonIndices) {
  auto mixedMesh = Testdata::getMixedSampleMesh();
  std::vector<std::unordered_set<std::size_t>> expectedAttachedPolygonIndices{
      {0, 6},           // 0
      {0, 1, 2},        // 1
      {2},              // 2
      {2},              // 3
      {2, 3},           // 4
      {3},              // 5
      {3, 4, 5},        // 6
      {5},              // 7
      {5, 6},           // 8
      {1, 2, 3, 4},     // 9
      {0, 1, 4, 5, 6},  // 10
  };

  for (std::size_t nodeIndex = 0; nodeIndex < mixedMesh.getNumberOfNodes();
       ++nodeIndex) {
    EXPECT_EQ(expectedAttachedPolygonIndices.at(nodeIndex),
              mixedMesh.getAttachedPolygonIndices(nodeIndex));
  }
}

TEST(PolygonalMesh, getIndicesOfNeighborPolygons) {
  auto mixedMesh = Testdata::getMixedSampleMesh();
  std::vector<std::unordered_set<std::size_t>>
      expectedIndicesOfNeighborPolygons{
          {1, 2, 4, 5, 6},     // 0
          {0, 2, 3, 4, 5, 6},  // 1
          {0, 1, 3, 4},        // 2
          {1, 2, 4, 5},        // 3
          {0, 1, 2, 3, 5, 6},  // 4
          {0, 1, 3, 4, 6},     // 5
          {0, 1, 4, 5},        // 6
      };

  for (std::size_t polygonIndex = 0;
       polygonIndex < mixedMesh.getNumberOfPolygons(); ++polygonIndex) {
    EXPECT_EQ(expectedIndicesOfNeighborPolygons.at(polygonIndex),
              mixedMesh.getIndicesOfNeighborPolygons(polygonIndex));
  }
}
