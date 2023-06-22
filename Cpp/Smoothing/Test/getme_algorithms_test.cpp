// Unit tests for GETMe algorithm variants.
#include "Smoothing/getme_algorithms.h"

#include "Mathematics/generalized_polygon_transformation.h"
#include "Mesh/polygonal_mesh.h"
#include "Mesh/polygonal_mesh_algorithms.h"
#include "Smoothing/basic_getme_simultaneous_config.h"
#include "Smoothing/default_configuration.h"
#include "Smoothing/getme_config.h"
#include "Smoothing/getme_result.h"
#include "Smoothing/getme_sequential_config.h"
#include "Smoothing/getme_simultaneous_config.h"
#include "Smoothing/laplace_algorithms.h"
#include "Smoothing/smoothing_result.h"
#include "Testdata/meshes.h"

#include "gtest/gtest.h"

#include <cmath>
#include <numbers>

namespace {
Mathematics::GeneralizedPolygonTransformation
getNonRegularizingPentagonTransformation() {
  const double lambda = 0.5;
  const double theta = 0.99 * std::numbers::pi / 2.0;
  return Mathematics::GeneralizedPolygonTransformation(lambda, theta);
}
}  // namespace

TEST(GetmeAlgorithms,
     basicGetmeSimultaneous_throwIfTransformationsNotRegularizing) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  const double maxNodeRelocationDistanceThreshold = 0.01;
  Smoothing::BasicGetmeSimultaneousConfig config(
      maxNodeRelocationDistanceThreshold,
      initialMesh.getMaximalNumberOfPolygonNodes());
  config.polygonTransformations.at(5) =
      getNonRegularizingPentagonTransformation();

  EXPECT_ANY_THROW(Smoothing::basicGetmeSimultaneous(initialMesh, config));
}

TEST(GetmeAlgorithms, basicGetmeSimultaneous_maxIterTerminated) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  auto expectedNodes = initialMesh.getNodes();
  expectedNodes.at(9) = {6.2852970146975231, 1.826704003984736};
  expectedNodes.at(10) = {3.3274646701939128, 1.875503321745196};
  const Mesh::PolygonalMesh expectedMesh(expectedNodes,
                                         initialMesh.getPolygons(),
                                         initialMesh.getFixedNodeIndices());
  const double maxNodeRelocationDistanceThreshold = 0.0;
  Smoothing::BasicGetmeSimultaneousConfig config(
      maxNodeRelocationDistanceThreshold,
      initialMesh.getMaximalNumberOfPolygonNodes());
  config.maxIterations = 5;

  const auto basicGetmeSimultaneousResult =
      Smoothing::basicGetmeSimultaneous(initialMesh, config);

  EXPECT_EQ(config.maxIterations, basicGetmeSimultaneousResult.iterations);
  const double nodeTolerance = 1.0e-15;
  EXPECT_TRUE(Mesh::areEqual(expectedMesh, basicGetmeSimultaneousResult.mesh,
                             nodeTolerance));
}

TEST(GetmeAlgorithms, basicGetmeSimultaneous_maxRelocationTerminated) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  auto expectedNodes = initialMesh.getNodes();
  expectedNodes.at(9) = {6.2295311988930937, 1.8091846592883019};
  expectedNodes.at(10) = {3.2460592185108261, 1.7207683695711489};
  const Mesh::PolygonalMesh expectedMesh(expectedNodes,
                                         initialMesh.getPolygons(),
                                         initialMesh.getFixedNodeIndices());
  const double maxNodeRelocationDistanceThreshold = 0.2;
  Smoothing::BasicGetmeSimultaneousConfig config(
      maxNodeRelocationDistanceThreshold,
      initialMesh.getMaximalNumberOfPolygonNodes());

  const auto basicGetmeSimultaneousResult =
      Smoothing::basicGetmeSimultaneous(initialMesh, config);

  EXPECT_EQ(3, basicGetmeSimultaneousResult.iterations);
  const double nodeTolerance = 1.0e-15;
  EXPECT_TRUE(Mesh::areEqual(expectedMesh, basicGetmeSimultaneousResult.mesh,
                             nodeTolerance));
}

TEST(GetmeAlgorithms, getmeSimultaneous_throwIfTransformationsNotRegularizing) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  Smoothing::GetmeSimultaneousConfig config(
      initialMesh.getMaximalNumberOfPolygonNodes());
  config.polygonTransformations.at(5) =
      getNonRegularizingPentagonTransformation();

  EXPECT_ANY_THROW(Smoothing::getmeSimultaneous(initialMesh, config));
}

TEST(GetmeAlgorithms, getmeSimultaneous_throwIfMeshIsInvalid) {
  const auto invalidInitialMesh = Testdata::getInvalidMixedSampleMesh();
  const Smoothing::GetmeSimultaneousConfig config(
      invalidInitialMesh.getMaximalNumberOfPolygonNodes());
  EXPECT_ANY_THROW(Smoothing::getmeSimultaneous(invalidInitialMesh, config));
}

TEST(GetmeAlgorithms, getmeSimultaneous_maxIterTerminated) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  auto expectedNodes = initialMesh.getNodes();
  expectedNodes.at(9) = {6.1828573161257783, 1.8204182075801358};
  expectedNodes.at(10) = {3.1837189214806196, 1.5770535411879405};
  const Mesh::PolygonalMesh expectedMesh(expectedNodes,
                                         initialMesh.getPolygons(),
                                         initialMesh.getFixedNodeIndices());
  Smoothing::GetmeSimultaneousConfig config(
      initialMesh.getMaximalNumberOfPolygonNodes());
  config.qMeanImprovementThreshold = 0.0;
  config.maxIterations = 2;

  const auto getmeSimultaneousResult =
      Smoothing::getmeSimultaneous(initialMesh, config);

  EXPECT_EQ(config.maxIterations, getmeSimultaneousResult.iterations);
  const double nodeTolerance = 1.0e-15;
  EXPECT_TRUE(Mesh::areEqual(expectedMesh, getmeSimultaneousResult.mesh,
                             nodeTolerance));
}

TEST(GetmeAlgorithms, getmeSimultaneous_qualityTerminated) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  auto expectedNodes = initialMesh.getNodes();
  expectedNodes.at(9) = {6.2295311988930937, 1.8091846592883019};
  expectedNodes.at(10) = {3.2460592185108261, 1.7207683695711489};
  const Mesh::PolygonalMesh expectedMesh(expectedNodes,
                                         initialMesh.getPolygons(),
                                         initialMesh.getFixedNodeIndices());
  Smoothing::GetmeSimultaneousConfig config(
      initialMesh.getMaximalNumberOfPolygonNodes());
  config.qMeanImprovementThreshold = 0.01;

  const auto getmeSimultaneousResult =
      Smoothing::getmeSimultaneous(initialMesh, config);

  EXPECT_EQ(3, getmeSimultaneousResult.iterations);
  const double nodeTolerance = 1.0e-15;
  EXPECT_TRUE(Mesh::areEqual(expectedMesh, getmeSimultaneousResult.mesh,
                             nodeTolerance));
}

TEST(GetmeAlgorithms, getmeSequential_throwIfTransformationsNotRegularizing) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  Smoothing::GetmeSequentialConfig config(
      initialMesh.getMaximalNumberOfPolygonNodes());
  config.polygonTransformations.at(5) =
      getNonRegularizingPentagonTransformation();

  EXPECT_ANY_THROW(Smoothing::getmeSequential(initialMesh, config));
}

TEST(GetmeAlgorithms, getmeSequential_throwIfMeshIsInvalid) {
  const auto invalidInitialMesh = Testdata::getInvalidMixedSampleMesh();
  const Smoothing::GetmeSequentialConfig config(
      invalidInitialMesh.getMaximalNumberOfPolygonNodes());
  EXPECT_ANY_THROW(Smoothing::getmeSequential(invalidInitialMesh, config));
}

TEST(GetmeAlgorithms, getmeSequential_throwIfCycleLengthInvalid) {
  const auto invalidInitialMesh = Testdata::getInvalidMixedSampleMesh();
  Smoothing::GetmeSequentialConfig config(
      invalidInitialMesh.getMaximalNumberOfPolygonNodes());
  config.maxIterations = 10;
  config.maxNoImprovementCycles = 11;
  EXPECT_ANY_THROW(Smoothing::getmeSequential(invalidInitialMesh, config));
}

TEST(GetmeAlgorithms, getmeSequential_maxIterTerminated) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  auto expectedNodes = initialMesh.getNodes();
  expectedNodes.at(9) = {6.1352858730635251, 1.9332152696305898};
  expectedNodes.at(10) = {3.7873558943916716, 1.5418381939168795};
  const Mesh::PolygonalMesh expectedMesh(expectedNodes,
                                         initialMesh.getPolygons(),
                                         initialMesh.getFixedNodeIndices());

  Smoothing::GetmeSequentialConfig config(
      initialMesh.getMaximalNumberOfPolygonNodes());
  config.maxIterations = 500;

  const auto getmeSequentialResult =
      Smoothing::getmeSequential(initialMesh, config);

  EXPECT_EQ(config.maxIterations, getmeSequentialResult.iterations);
  const double nodeTolerance = 1.0e-15;
  EXPECT_TRUE(
      Mesh::areEqual(expectedMesh, getmeSequentialResult.mesh, nodeTolerance));
}

TEST(GetmeAlgorithms, getmeSequential_qualityTerminated) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  auto expectedNodes = initialMesh.getNodes();
  expectedNodes.at(9) = {6.2060523653413071, 1.9112677103218558};
  expectedNodes.at(10) = {3.9772068085161578, 1.5627349353413904};
  const Mesh::PolygonalMesh expectedMesh(expectedNodes,
                                         initialMesh.getPolygons(),
                                         initialMesh.getFixedNodeIndices());

  Smoothing::GetmeSequentialConfig config(
      initialMesh.getMaximalNumberOfPolygonNodes());

  const auto getmeSequentialResult =
      Smoothing::getmeSequential(initialMesh, config);

  EXPECT_EQ(3800, getmeSequentialResult.iterations);
  const double nodeTolerance = 1.0e-15;
  EXPECT_TRUE(
      Mesh::areEqual(expectedMesh, getmeSequentialResult.mesh, nodeTolerance));
}

TEST(GetmeAlgorithms, getme_throwIfMeshIsInvalid) {
  const auto invalidInitialMesh = Testdata::getInvalidMixedSampleMesh();
  const Smoothing::GetmeConfig getmeConfig(
      invalidInitialMesh.getMaximalNumberOfPolygonNodes());
  EXPECT_ANY_THROW(Smoothing::getme(invalidInitialMesh, getmeConfig));
}

TEST(GetmeAlgorithms, getme) {
  const auto initialMesh = Testdata::getMixedSampleMesh();
  auto expectedNodes = initialMesh.getNodes();
  expectedNodes.at(9) = {6.3356720528397403, 1.8744625909694248};
  expectedNodes.at(10) = {3.9787712483994113, 1.5627335182809372};
  const Mesh::PolygonalMesh expectedMesh(expectedNodes,
                                         initialMesh.getPolygons(),
                                         initialMesh.getFixedNodeIndices());
  const Smoothing::GetmeConfig getmeConfig(
      initialMesh.getMaximalNumberOfPolygonNodes());

  const auto getmeResult = Smoothing::getme(initialMesh, getmeConfig);

  EXPECT_EQ(10, getmeResult.getmeSimultaneousIterations);
  EXPECT_EQ(3800, getmeResult.getmeSequentialIterations);
  const double nodeTolerance = 1.0e-15;
  EXPECT_TRUE(Mesh::areEqual(expectedMesh, getmeResult.mesh, nodeTolerance));
}
