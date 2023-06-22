// Unit tests for smoothing algorithm building blocks.
#include "../Source/common_algorithms.h"

#include "Mathematics/generalized_polygon_transformation.h"
#include "Mathematics/polygon.h"
#include "Mathematics/vector2d.h"
#include "Mathematics/vector2d_algorithms.h"
#include "Mesh/mesh_quality.h"
#include "Mesh/polygonal_mesh.h"
#include "Mesh/polygonal_mesh_algorithms.h"
#include "Testdata/meshes.h"

#include "gtest/gtest.h"

#include <numbers>

TEST(CommonAlgorithms, applyEdgeLengthScaling) {
  const Mathematics::Polygon polygon({0, 1, 2, 3, 4, 5});
  const std::vector<Mathematics::Vector2D> meshNodes{
      {3.0, 5.0},  {-9.0, -2.0}, {7.0, 3.0},
      {9.0, -7.0}, {4.0, 4.0},   {5.0, -9.0},
  };
  std::vector<Mathematics::Vector2D> transformedNodes{
      {2.120029401627464, 3.050931073468095},
      {-3.993368337253149, -0.308356785127020},
      {3.629849854773023, -2.437418888363173},
      {7.820578277134416, -2.594900517549698},
      {4.845821607436489, 0.038824027685005},
      {4.577089196281754, -3.749078910113208},
  };
  const std::vector<Mathematics::Vector2D> expectedNodes{
      {0.706538394108473, 8.521742056117311},
      {-13.663046103965184, 0.625712254157441},
      {4.255382016765926, -4.378663234046956},
      {14.105718727334228, -4.748824914040057},
      {7.113532196663723, 1.441763203056728},
      {6.481874769092831, -7.461729365244467},
  };
  Smoothing::applyEdgeLengthScaling(polygon, meshNodes, transformedNodes);
  const double tolerance = 1.0e-14;
  EXPECT_TRUE(
      Mathematics::areEqual(expectedNodes, transformedNodes, tolerance));
}

TEST(CommonAlgorithms, updateMaxSquaredNodeRelocationDistance) {
  const Mathematics::Vector2D oldNode(3.0, -2.0);
  const Mathematics::Vector2D newNode(1.0, 2.0);
  double maxSquaredNodeRelocationDistance = 0.0;

  Smoothing::updateMaxSquaredNodeRelocationDistance(
      oldNode, newNode, maxSquaredNodeRelocationDistance);

  EXPECT_EQ(20.0, maxSquaredNodeRelocationDistance);

  const double largerMaxValue = 100.0;
  maxSquaredNodeRelocationDistance = largerMaxValue;
  Smoothing::updateMaxSquaredNodeRelocationDistance(
      oldNode, newNode, maxSquaredNodeRelocationDistance);

  EXPECT_EQ(largerMaxValue, maxSquaredNodeRelocationDistance);
}

TEST(CommonAlgorithms, transformAndScaleElement) {
  const double lambda = 0.3;
  const double theta = std::numbers::pi / 5.0;
  const Mathematics::GeneralizedPolygonTransformation transformation(lambda,
                                                                     theta);
  const Mathematics::Polygon polygon({0, 1, 2, 3, 4});
  const std::vector<Mathematics::Vector2D> meshNodes{
      {6.0, -8.0}, {8.0, -4.0}, {-7.0, 1.0}, {8.0, 9.0}, {3.0, 9.0},
  };
  const std::vector<Mathematics::Vector2D> expectedNodes{
      {+0.955412051458474, -6.836136432133172},
      {+9.828471223820758, +2.138754261004771},
      {+0.138965075543568, +1.053163587137887},
      {+9.598106992441132, +3.244484494788414},
      {-2.520955343263935, +7.399734089202100},
  };
  const auto transformedAndScaledNodes =
      Smoothing::transformAndScaleElement(transformation, polygon, meshNodes);
  const double tolerance = 1.0e-14;
  EXPECT_TRUE(Mathematics::areEqual(expectedNodes, transformedAndScaledNodes,
                                    tolerance));
}

TEST(CommonAlgorithms, transformScaleAndRelaxElement) {
  const double lambda = 0.3;
  const double theta = std::numbers::pi / 5.0;
  const Mathematics::GeneralizedPolygonTransformation transformation(lambda,
                                                                     theta);
  const Mathematics::Polygon polygon({0, 1, 2, 3, 4});
  const std::vector<Mathematics::Vector2D> meshNodes{
      {6.0, -8.0}, {8.0, -4.0}, {-7.0, 1.0}, {8.0, 9.0}, {3.0, 9.0},
  };
  const std::vector<Mathematics::Vector2D> expectedNodes{
      {+2.468788436020932, -7.185295502493220},
      {+9.279929856674531, +0.297127982703340},
      {-2.002724447119503, +1.037214510996521},
      {+9.118674894708793, +4.971139146351890},
      {-0.864668740284754, +7.879813862441470},
  };
  const double relaxationFactorRho = 0.7;
  const auto transformedAndScaledNodes =
      Smoothing::transformScaleAndRelaxElement(
          transformation, relaxationFactorRho, polygon, meshNodes);
  const double tolerance = 1.0e-14;
  EXPECT_TRUE(Mathematics::areEqual(expectedNodes, transformedAndScaledNodes,
                                    tolerance));
}

TEST(
    CommonAlgorithms,
    iterativelyResetNodesResultingInInvalidElementsSetNewMeshNodesAndUpdateElementQualityNumbers) {
  auto mesh = Testdata::getMixedSampleMesh();
  auto meanRatioNumbers = Mesh::computeMeanRatioQualityNumberOfPolygons(
      mesh.getPolygons(), mesh.getNodes());
  // Modify nodes 9 and 10. Node 9 modification invalidates elements.
  auto newNodePositions = mesh.getNodes();
  newNodePositions.at(9) = {12.0, 3.0};
  newNodePositions.at(10) = {4.0, 3.0};
  auto expectedMesh = mesh;
  // Expected mesh has only node 10 changed.
  expectedMesh.getMutableNodes().at(10) = newNodePositions.at(10);
  const auto expectedMeanRatioNumbers =
      Mesh::computeMeanRatioQualityNumberOfPolygons(expectedMesh.getPolygons(),
                                                    expectedMesh.getNodes());
  const Mesh::MeshQuality expectedMeshQuality(expectedMesh);

  const auto meshQuality = Smoothing::
      iterativelyResetNodesResultingInInvalidElementsSetNewMeshNodesAndUpdateElementQualityNumbers(
          newNodePositions, meanRatioNumbers, mesh);

  // Expect matching quality result.
  EXPECT_EQ(expectedMeshQuality.getQMin(), meshQuality.getQMin());
  EXPECT_EQ(expectedMeshQuality.getQMean(), meshQuality.getQMean());
  // Expect only reduced quality evaluation.
  EXPECT_FALSE(meshQuality.getNumberOfInvalidElements().has_value());
  EXPECT_FALSE(meshQuality.getQMinStar().has_value());
  // Expect matching modified input parameters.
  EXPECT_TRUE(Mesh::areEqual(expectedMesh, mesh, 0.0));
  EXPECT_EQ(expectedMeanRatioNumbers, meanRatioNumbers);
  EXPECT_EQ(expectedMesh.getNodes(), newNodePositions);
}

TEST(CommonAlgorithms, checkTransformations_validTransformations) {
  const auto maxNumberOfPolygonNodes = 5;
  std::vector<Mathematics::GeneralizedPolygonTransformation> transformations;
  transformations.reserve(maxNumberOfPolygonNodes + 1);
  for (std::size_t numberOfPolygonNodes = 0;
       numberOfPolygonNodes <= maxNumberOfPolygonNodes;
       ++numberOfPolygonNodes) {
    transformations.emplace_back(numberOfPolygonNodes);
  }

  EXPECT_NO_THROW(Smoothing::checkTransformations(maxNumberOfPolygonNodes,
                                                  transformations));
}

TEST(CommonAlgorithms, checkTransformations_throwIfTransformationsMissing) {
  const auto maxNumberOfPolygonNodes = 5;
  std::vector<Mathematics::GeneralizedPolygonTransformation> transformations;
  for (std::size_t numberOfPolygonNodes = 0;
       numberOfPolygonNodes < maxNumberOfPolygonNodes; ++numberOfPolygonNodes) {
    transformations.emplace_back(numberOfPolygonNodes);
  }

  EXPECT_ANY_THROW(Smoothing::checkTransformations(maxNumberOfPolygonNodes,
                                                   transformations));
}

TEST(CommonAlgorithms, checkTransformations_throwIfNotRegularizing) {
  const auto maxNumberOfPolygonNodes = 5;
  std::vector<Mathematics::GeneralizedPolygonTransformation> transformations;
  transformations.reserve(maxNumberOfPolygonNodes + 1);
  for (std::size_t numberOfPolygonNodes = 0;
       numberOfPolygonNodes < maxNumberOfPolygonNodes; ++numberOfPolygonNodes) {
    transformations.emplace_back(numberOfPolygonNodes);
  }
  // Add non regularizing pentagon transformation.
  const double lambda = 0.5;
  const double theta = 0.99 * std::numbers::pi / 2.0;
  transformations.emplace_back(lambda, theta);

  EXPECT_ANY_THROW(Smoothing::checkTransformations(maxNumberOfPolygonNodes,
                                                   transformations));
}

TEST(CommonAlgorithms, checkTransformations_meshVariant) {
  const auto mesh = Testdata::getMixedSampleMesh();
  const auto maxNumberOfPolygonNodes = mesh.getMaximalNumberOfPolygonNodes();
  std::vector<Mathematics::GeneralizedPolygonTransformation> transformations;
  transformations.reserve(maxNumberOfPolygonNodes + 1);
  for (std::size_t numberOfPolygonNodes = 0;
       numberOfPolygonNodes <= maxNumberOfPolygonNodes;
       ++numberOfPolygonNodes) {
    transformations.emplace_back(numberOfPolygonNodes);
  }

  EXPECT_NO_THROW(Smoothing::checkTransformations(mesh, transformations));
}
