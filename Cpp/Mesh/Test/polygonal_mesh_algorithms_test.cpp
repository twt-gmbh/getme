/*
Unit tests for planar polygonal mesh algorithms.

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
#include "Mesh/polygonal_mesh_algorithms.h"

#include "Mathematics/vector2d_algorithms.h"
#include "Mesh/polygonal_mesh.h"
#include "Testdata/meshes.h"

#include "gtest/gtest.h"

#include <algorithm>
#include <filesystem>
#include <string>

TEST(PolygonalMeshAlgorithms, meshIoRoundtrip) {
  const std::string testSuiteName = ::testing::UnitTest::GetInstance()
                                        ->current_test_info()
                                        ->test_suite_name();
  const std::string testName =
      ::testing::UnitTest::GetInstance()->current_test_info()->name();
  std::string filename =
      "tmp_unittest_" + testSuiteName + "_" + testName + ".mesh";
  std::ranges::transform(filename.begin(), filename.end(), filename.begin(),
                         [](auto c) { return std::tolower(c); });

  auto filePath = std::filesystem::temp_directory_path() / filename;
  if (std::filesystem::exists(filePath)) {
    std::filesystem::remove(filePath);
  }

  const auto originalMesh = Testdata::getMixedSampleMesh();

  Mesh::writeMeshFile(originalMesh, filePath);
  const auto importedMesh = Mesh::readMeshFile(filePath);
  std::filesystem::remove(filePath);

  const double nodesEqualTolerance = 1.0e-15;
  EXPECT_TRUE(Mesh::areEqual(originalMesh, importedMesh, nodesEqualTolerance));
}

TEST(PolygonalMeshAlgorithms, readMeshFile_throwIfNotFound) {
  EXPECT_ANY_THROW(
      Mesh::readMeshFile("_t_h_i_s_m_e_s_h_d_o_e_s_n_o_t_e_x_i_s_t_"));
}

TEST(PolygonalMeshAlgorithms,
     computeMeanRatioQualityNumberOfPolygons_vectorArgument) {
  const auto mesh = Testdata::getMixedSampleMesh();
  const std::size_t numberOfPolygons = mesh.getNumberOfPolygons();
  const std::vector<double> expectedMeanRatioQualityNumbers{
      6.298366572977736e-01, 9.897433186107867e-01, 9.141634485276990e-01,
      1.000000000000000e+00, 7.085662394599952e-01, 7.804455606313192e-01,
      9.742785792574934e-01,
  };
  std::vector<double> meanRatioQualityNumbers(numberOfPolygons, 0.0);
  Mesh::computeMeanRatioQualityNumberOfPolygons(
      mesh.getPolygons(), mesh.getNodes(), meanRatioQualityNumbers);
  const double tolerance = 1.0e-15;
  for (std::size_t index = 0; index < numberOfPolygons; ++index) {
    EXPECT_NEAR(expectedMeanRatioQualityNumbers.at(index),
                meanRatioQualityNumbers.at(index), tolerance);
  }
}

TEST(PolygonalMeshAlgorithms,
     computeMeanRatioQualityNumberOfPolygon_returningVector) {
  const auto mesh = Testdata::getMixedSampleMesh();
  std::vector<double> expectedMeanRatioQualityNumbers(
      mesh.getNumberOfPolygons(), -1.0);
  Mesh::computeMeanRatioQualityNumberOfPolygons(
      mesh.getPolygons(), mesh.getNodes(), expectedMeanRatioQualityNumbers);
  const auto meanRatioQualityNumbers =
      Mesh::computeMeanRatioQualityNumberOfPolygons(mesh.getPolygons(),
                                                    mesh.getNodes());
  EXPECT_EQ(expectedMeanRatioQualityNumbers, meanRatioQualityNumbers);
}

TEST(PolygonalMeshAlgorithms,
     computeMeanRatioQualityNumberOfPolygons_throwsOnSizeMismatch) {
  const auto mesh = Testdata::getMixedSampleMesh();
  std::vector<double> meanRatioQualityNumbers(3, 0.0);  // insufficient length
  EXPECT_ANY_THROW(Mesh::computeMeanRatioQualityNumberOfPolygons(
      mesh.getPolygons(), mesh.getNodes(), meanRatioQualityNumbers));
}

TEST(PolygonalMeshAlgorithms, areEqual_sameMesh) {
  const auto mesh = Testdata::getMixedSampleMesh();
  EXPECT_TRUE(Mesh::areEqual(mesh, mesh));
  EXPECT_TRUE(Mesh::areEqual(mesh, mesh, 0.0));
  EXPECT_TRUE(Mesh::areEqual(mesh, mesh, 17.0));
}

TEST(PolygonalMeshAlgorithms, areEqual_nodesDifferWithinTolerance) {
  const double tolerance = 0.1;
  const auto expectedMesh = Testdata::getMixedSampleMesh();
  auto nodes = Testdata::getMixedSampleMeshNodes();
  nodes.front() = nodes.front() + Mathematics::Vector2D(tolerance / 2.0, 0.0);
  const auto polygons = Testdata::getMixedSampleMeshPolygons();
  const auto fixedNodeIndices = Testdata::getMixedSampleMeshFixedNodeIndices();

  const Mesh::PolygonalMesh mesh(nodes, polygons, fixedNodeIndices);
  EXPECT_TRUE(Mesh::areEqual(expectedMesh, mesh, tolerance));
}

TEST(PolygonalMeshAlgorithms, areEqual_nodesDiffer) {
  const double tolerance = 0.1;
  const auto expectedMesh = Testdata::getMixedSampleMesh();
  auto nodes = Testdata::getMixedSampleMeshNodes();
  nodes.front() = nodes.front() + Mathematics::Vector2D(tolerance, tolerance);
  const auto polygons = Testdata::getMixedSampleMeshPolygons();
  const auto fixedNodeIndices = Testdata::getMixedSampleMeshFixedNodeIndices();

  const Mesh::PolygonalMesh mesh(nodes, polygons, fixedNodeIndices);
  EXPECT_FALSE(Mesh::areEqual(expectedMesh, mesh, tolerance));
}

TEST(PolygonalMeshAlgorithms, areEqual_polygonsDiffer) {
  const auto expectedMesh = Testdata::getMixedSampleMesh();
  const auto nodes = Testdata::getMixedSampleMeshNodes();
  auto polygons = Testdata::getMixedSampleMeshPolygons();
  polygons.pop_back();
  const auto fixedNodeIndices = Testdata::getMixedSampleMeshFixedNodeIndices();

  const Mesh::PolygonalMesh mesh(nodes, polygons, fixedNodeIndices);
  EXPECT_FALSE(Mesh::areEqual(expectedMesh, mesh));
}

TEST(PolygonalMeshAlgorithms, areEqual_fixedNodesDiffer) {
  const auto expectedMesh = Testdata::getMixedSampleMesh();
  const auto nodes = Testdata::getMixedSampleMeshNodes();
  const auto polygons = Testdata::getMixedSampleMeshPolygons();
  const std::unordered_set<std::size_t> fixedNodeIndices{0, 1, 2};

  const Mesh::PolygonalMesh mesh(nodes, polygons, fixedNodeIndices);
  EXPECT_FALSE(Mesh::areEqual(expectedMesh, mesh));
}

TEST(PolygonalMeshAlgorithms, distortNodesLocally) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  const double maxDistortionRadius = 4.0;
  const auto distortedMesh =
      Mesh::distortNodesLocally(initialMesh, maxDistortionRadius);

  EXPECT_EQ(initialMesh.getPolygons(), distortedMesh.getPolygons());
  EXPECT_EQ(initialMesh.getFixedNodeIndices(),
            distortedMesh.getFixedNodeIndices());

  EXPECT_EQ(initialMesh.getNumberOfNodes(), distortedMesh.getNumberOfNodes());
  const auto& fixedNodeIndices = initialMesh.getFixedNodeIndices();
  for (std::size_t nodeIndex = 0; nodeIndex < initialMesh.getNumberOfNodes();
       ++nodeIndex) {
    const double expectedMaxDistance =
        fixedNodeIndices.contains(nodeIndex) ? 0.0 : maxDistortionRadius;
    const double distance = (initialMesh.getNodes().at(nodeIndex)
                             - distortedMesh.getNodes().at(nodeIndex))
                                .getLength();
    EXPECT_LE(distance, expectedMaxDistance);
  }
}

TEST(PolygonalMeshAlgorithms, distortNodesLocally_throwIfRadiusInvalid) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  EXPECT_ANY_THROW(Mesh::distortNodesLocally(initialMesh, -1.0));
  EXPECT_ANY_THROW(Mesh::distortNodesLocally(initialMesh, 0.0));
}
