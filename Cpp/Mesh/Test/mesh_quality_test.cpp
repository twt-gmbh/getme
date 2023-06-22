// Unit tests for mesh quality info objects.
#include "Mesh/mesh_quality.h"

#include "Mathematics/polygon_algorithms.h"
#include "Mesh/polygonal_mesh.h"
#include "Mesh/polygonal_mesh_algorithms.h"
#include "Testdata/meshes.h"

#include "gtest/gtest.h"

namespace {
const double expectedSampleMeshQMin = 6.298366572977736e-01;
const double expectedSampleMeshQMinStar = expectedSampleMeshQMin;
const double expectedSampleMeshQMean = 8.567191148264383e-01;
const double qualityTolerance = 1.0e-15;
const std::vector<double> invalidMeshMeanRatioNumbers{
    // Actual values do not matter, but two elements have to be invalid.
    0.1, 0.2, -1.0, -1.0, 0.3, 0.4, 0.5,
};

std::vector<double> getSampleMeshElementMeanRatioNumbers() {
  const auto mesh = Testdata::getMixedSampleMesh();
  return Mesh::computeMeanRatioQualityNumberOfPolygons(mesh.getPolygons(),
                                                       mesh.getNodes());
};
}  // namespace

TEST(MeshQuality, meshConstructorValidMesh) {
  const auto mesh = Testdata::getMixedSampleMesh();
  const Mesh::MeshQuality meshQuality(mesh);
  EXPECT_NEAR(expectedSampleMeshQMin, meshQuality.getQMin(), qualityTolerance);
  EXPECT_NEAR(expectedSampleMeshQMinStar, meshQuality.getQMinStar().value(),
              qualityTolerance);
  EXPECT_NEAR(expectedSampleMeshQMean, meshQuality.getQMean(),
              qualityTolerance);
  EXPECT_EQ(0, meshQuality.getNumberOfInvalidElements().value());
  EXPECT_TRUE(meshQuality.isValidMesh());
}

TEST(MeshQuality, qualityConstructorValidMeshWithoutValidCount) {
  const bool determineNumberOfInvalidElements = false;
  const Mesh::MeshQuality meshQuality(getSampleMeshElementMeanRatioNumbers(),
                                      determineNumberOfInvalidElements);
  EXPECT_NEAR(expectedSampleMeshQMin, meshQuality.getQMin(), qualityTolerance);
  EXPECT_FALSE(meshQuality.getQMinStar().has_value());
  EXPECT_NEAR(expectedSampleMeshQMean, meshQuality.getQMean(),
              qualityTolerance);
  EXPECT_FALSE(meshQuality.getNumberOfInvalidElements().has_value());
  EXPECT_TRUE(meshQuality.isValidMesh());
}

TEST(MeshQuality, qualityConstructorValidMeshWithValidCount) {
  const bool determineNumberOfInvalidElements = true;
  const Mesh::MeshQuality meshQuality(getSampleMeshElementMeanRatioNumbers(),
                                      determineNumberOfInvalidElements);
  EXPECT_NEAR(expectedSampleMeshQMin, meshQuality.getQMin(), qualityTolerance);
  EXPECT_FALSE(meshQuality.getQMinStar().has_value());
  EXPECT_NEAR(expectedSampleMeshQMean, meshQuality.getQMean(),
              qualityTolerance);
  EXPECT_EQ(0, meshQuality.getNumberOfInvalidElements().value());
  EXPECT_TRUE(meshQuality.isValidMesh());
}

TEST(MeshQuality, meshConstructorInvalidMesh) {
  const auto mesh = Testdata::getInvalidMixedSampleMesh();
  const Mesh::MeshQuality meshQuality(mesh);
  EXPECT_EQ(-1.0, meshQuality.getQMin());
  EXPECT_FALSE(meshQuality.getQMinStar().has_value());
  EXPECT_EQ(-1.0, meshQuality.getQMean());
  EXPECT_TRUE(meshQuality.getNumberOfInvalidElements().has_value());
  EXPECT_EQ(2, meshQuality.getNumberOfInvalidElements().value());
  EXPECT_FALSE(meshQuality.isValidMesh());
}

TEST(MeshQuality, qualityConstructorInvalidMeshWithoutValidCount) {
  const bool determineNumberOfInvalidElements = false;
  const Mesh::MeshQuality meshQuality(invalidMeshMeanRatioNumbers,
                                      determineNumberOfInvalidElements);
  EXPECT_EQ(-1.0, meshQuality.getQMin());
  EXPECT_FALSE(meshQuality.getQMinStar().has_value());
  EXPECT_EQ(-1.0, meshQuality.getQMean());
  EXPECT_FALSE(meshQuality.getNumberOfInvalidElements().has_value());
  EXPECT_FALSE(meshQuality.isValidMesh());
}

TEST(MeshQuality, qualityConstructorInvalidMeshWithValidCount) {
  const bool determineNumberOfInvalidElements = true;
  const Mesh::MeshQuality meshQuality(invalidMeshMeanRatioNumbers,
                                      determineNumberOfInvalidElements);
  EXPECT_EQ(-1.0, meshQuality.getQMin());
  EXPECT_FALSE(meshQuality.getQMinStar().has_value());
  EXPECT_EQ(-1.0, meshQuality.getQMean());
  EXPECT_TRUE(meshQuality.getNumberOfInvalidElements().has_value());
  EXPECT_EQ(2, meshQuality.getNumberOfInvalidElements().value());
  EXPECT_FALSE(meshQuality.isValidMesh());
}

TEST(MeshQuality, qMinAndQMinStarDiffer) {
  const auto nodes = Testdata::getMixedSampleMeshNodes();
  const auto polygons = Testdata::getMixedSampleMeshPolygons();
  // Additionally fix nodes 9 and 10, but unfix 4. Thus polygons 2 and 3 are non
  // fixed. q_min* polygon index is 2 in this case.
  auto fixedNodeIndies = Testdata::getMixedSampleMeshFixedNodeIndices();
  fixedNodeIndies.insert(9);
  fixedNodeIndies.insert(10);
  fixedNodeIndies.erase(4);
  const Mesh::PolygonalMesh mesh(nodes, polygons, fixedNodeIndies);
  const auto expectedMeanRatioQualityNumbers =
      getSampleMeshElementMeanRatioNumbers();
  const double expectedQminStar = expectedMeanRatioQualityNumbers.at(2);
  const Mesh::MeshQuality meshQuality(mesh);

  EXPECT_NEAR(expectedSampleMeshQMin, meshQuality.getQMin(), qualityTolerance);
  EXPECT_NEAR(expectedQminStar, meshQuality.getQMinStar().value(),
              qualityTolerance);
  EXPECT_NEAR(expectedSampleMeshQMean, meshQuality.getQMean(),
              qualityTolerance);
  EXPECT_EQ(0, meshQuality.getNumberOfInvalidElements().value());
  EXPECT_TRUE(meshQuality.isValidMesh());
}
