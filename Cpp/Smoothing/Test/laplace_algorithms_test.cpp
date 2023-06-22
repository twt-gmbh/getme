// Unit tests for Laplacian smoothing algorithm variants.
#include "Smoothing/laplace_algorithms.h"

#include "Mesh/mesh_quality.h"
#include "Mesh/polygonal_mesh.h"
#include "Mesh/polygonal_mesh_algorithms.h"
#include "Smoothing/basic_laplace_config.h"
#include "Smoothing/laplace_algorithms.h"
#include "Smoothing/smart_laplace_config.h"
#include "Smoothing/smoothing_result.h"
#include "Testdata/meshes.h"

#include "gtest/gtest.h"

#include <cmath>

namespace {
// Helper functions based on the mixed sample mesh.
double updateNode9PositionAndReturnSquaredRelocationDistance(
    const Mesh::PolygonalMesh& mesh,
    std::vector<Mathematics::Vector2D>& newNodes) {
  const auto& nodes = mesh.getNodes();
  const auto newPositionNode9 =
      (nodes.at(1) + nodes.at(4) + nodes.at(6) + nodes.at(10)) / 4.0;
  const double squaredRelocationDist =
      (nodes.at(9) - newPositionNode9).getLengthSquared();
  newNodes.at(9) = newPositionNode9;
  return squaredRelocationDist;
}

double updateNode10PositionAndReturnSquaredRelocationDistance(
    const Mesh::PolygonalMesh& mesh,
    std::vector<Mathematics::Vector2D>& newNodes) {
  const auto& nodes = mesh.getNodes();
  const auto newPositionNode10 =
      (nodes.at(0) + nodes.at(1) + nodes.at(9) + nodes.at(6) + nodes.at(8))
      / 5.0;
  const double squaredRelocationDist =
      (nodes.at(10) - newPositionNode10).getLengthSquared();
  newNodes.at(10) = newPositionNode10;
  return squaredRelocationDist;
}
}  // namespace

TEST(LaplaceAlgorithms, basicLaplace_maxIterTerminated) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  // Generate expected mesh by manual Laplace smoothing.
  auto expectedMesh = initialMesh;
  auto newNodes = expectedMesh.getNodes();
  // iteration 1
  updateNode9PositionAndReturnSquaredRelocationDistance(expectedMesh, newNodes);
  updateNode10PositionAndReturnSquaredRelocationDistance(expectedMesh,
                                                         newNodes);
  expectedMesh.setNodes(newNodes);

  // iteration 2
  updateNode9PositionAndReturnSquaredRelocationDistance(expectedMesh, newNodes);
  updateNode10PositionAndReturnSquaredRelocationDistance(expectedMesh,
                                                         newNodes);
  expectedMesh.setNodes(newNodes);

  // Set relocation distance threshold to zero to perform max iterations.
  const double maxNodeRelocationDistanceThreshold = 0.0;
  Smoothing::BasicLaplaceConfig config(maxNodeRelocationDistanceThreshold);
  config.maxIterations = 2;

  const auto laplaceResult = Smoothing::basicLaplace(initialMesh, config);

  const double tolerance = 1.0e-15;
  EXPECT_TRUE(Mesh::areEqual(expectedMesh, laplaceResult.mesh, tolerance));
  EXPECT_EQ(2, laplaceResult.iterations);
}

TEST(LaplaceAlgorithms, basicLaplace_thresholdTerminated) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  // Generate expected mesh by manual Laplace smoothing.
  auto expectedMesh = initialMesh;
  auto newNodes = expectedMesh.getNodes();
  // iteration 1
  updateNode9PositionAndReturnSquaredRelocationDistance(expectedMesh, newNodes);
  updateNode10PositionAndReturnSquaredRelocationDistance(expectedMesh,
                                                         newNodes);
  expectedMesh.getMutableNodes() = newNodes;

  // Set large relocation distance threshold to terminate after first iteration.
  const double maxNodeRelocationDistanceThreshold = 10.0;
  const Smoothing::BasicLaplaceConfig config(
      maxNodeRelocationDistanceThreshold);

  const auto laplaceResult = Smoothing::basicLaplace(initialMesh, config);

  const double tolerance = 1.0e-15;
  EXPECT_TRUE(Mesh::areEqual(expectedMesh, laplaceResult.mesh, tolerance));
  EXPECT_EQ(1, laplaceResult.iterations);
}

TEST(LaplaceAlgorithms, smartLaplace_requiresValidMesh) {
  const auto invalidMesh = Testdata::getInvalidMixedSampleMesh();
  const Smoothing::SmartLaplaceConfig config;

  EXPECT_ANY_THROW(Smoothing::smartLaplace(invalidMesh, config));
}

TEST(LaplaceAlgorithms, smartLaplace_maxIterTerminated) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  Smoothing::SmartLaplaceConfig config;
  config.maxIterations = 2;
  config.qMeanImprovementThreshold = 0.0;
  auto expectedMesh = initialMesh;
  // Node 10 movement improves mesh in the first iteration but not in the
  // second. Node 9 movement decreases mesh quality and is hence not conducted.
  updateNode10PositionAndReturnSquaredRelocationDistance(
      expectedMesh, expectedMesh.getMutableNodes());

  const auto smoothingResult = Smoothing::smartLaplace(initialMesh, config);

  EXPECT_EQ(config.maxIterations, smoothingResult.iterations);
  const double tolerance = 1.0e-15;
  EXPECT_TRUE(Mesh::areEqual(expectedMesh, smoothingResult.mesh, tolerance));
}

TEST(LaplaceAlgorithms, smartLaplace_qMeanTerminated) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  Smoothing::SmartLaplaceConfig config;
  // Set large qMean threshold to terminate after first iteration.
  config.qMeanImprovementThreshold = 0.1;
  // Only node 10 movement improves mesh.
  auto expectedMesh = initialMesh;
  updateNode10PositionAndReturnSquaredRelocationDistance(
      expectedMesh, expectedMesh.getMutableNodes());

  const auto smoothingResult = Smoothing::smartLaplace(initialMesh, config);

  EXPECT_EQ(1, smoothingResult.iterations);
  const double tolerance = 1.0e-15;
  EXPECT_TRUE(Mesh::areEqual(expectedMesh, smoothingResult.mesh, tolerance));
}
