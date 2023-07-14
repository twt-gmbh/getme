/*
Unit tests for generalized polygon transformations.

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
#include "Mathematics/generalized_polygon_transformation.h"

#include "Mathematics/polygon.h"
#include "Mathematics/vector2d.h"
#include "Mathematics/vector2d_algorithms.h"
#include "mathematics_test_utilities.h"

#include "gtest/gtest.h"

#include <cmath>
#include <numbers>

namespace {
// Transformed nodes computation according to definition 5.1 of the GETMe book.
// Used as alternative implementation for result comparison.
std::vector<Mathematics::Vector2D>
computeTransformedNodesUsingComplexRepresentation(
    const std::vector<Mathematics::Vector2D>& polygonNodes,
    const double lambda,
    const double theta) {
  const auto complexPolygonNodes =
      MathematicsTestUtilities::vector2DToComplexNodes(polygonNodes);
  const std::size_t numNodes = complexPolygonNodes.size();
  const std::complex<double> w(lambda, (1.0 - lambda) * std::tan(theta));
  const auto predecessorWeight = w * (1.0 - std::conj(w));
  const auto weight =
      std::abs(w) * std::abs(w) + std::abs(1.0 - w) * std::abs(1.0 - w);
  const auto successorWeight = std::conj(w) * (1.0 - w);

  std::vector<std::complex<double>> complexTransformedNodes(numNodes);
  for (std::size_t nodeNumber = 0; nodeNumber < numNodes; ++nodeNumber) {
    complexTransformedNodes.at(nodeNumber) =
        predecessorWeight
            * complexPolygonNodes.at((nodeNumber + numNodes - 1) % numNodes)
        + weight * complexPolygonNodes.at(nodeNumber)
        + successorWeight * complexPolygonNodes.at((nodeNumber + 1) % numNodes);
  }
  return MathematicsTestUtilities::complexToVector2DNodes(
      complexTransformedNodes);
}
}  // namespace

TEST(GeneralizedPolygonTransformation, lambdaThetaConstructorAndGetters) {
  const double lambda = 0.3;
  const double theta = std::numbers::pi / 3.0;
  const Mathematics::GeneralizedPolygonTransformation transformation(lambda,
                                                                     theta);
  EXPECT_EQ(lambda, transformation.getLambda());
  EXPECT_EQ(theta, transformation.getTheta());
}

TEST(GeneralizedPolygonTransformation, lambdaThetaConstructorThrows) {
  const double validLambda = 0.6;
  const double validTheta = std::numbers::pi / 5.0;
  EXPECT_NO_THROW(
      Mathematics::GeneralizedPolygonTransformation(validLambda, validTheta));

  for (const auto invalidLambda : {-1.0, 0.0, 1.0, 3.0}) {
    EXPECT_ANY_THROW(Mathematics::GeneralizedPolygonTransformation(
        invalidLambda, validTheta));
  }
  for (const auto invalidTheta : {-0.1, 0.0, std::numbers::pi / 2.0, 1.7}) {
    EXPECT_ANY_THROW(Mathematics::GeneralizedPolygonTransformation(
        validLambda, invalidTheta));
  }
}

TEST(GeneralizedPolygonTransformation, numberOfNodesConstructor) {
  for (std::size_t numberOfNodes = 3; numberOfNodes <= 10; ++numberOfNodes) {
    const double expectedLambda = 0.5;
    const double expectedTheta =
        std::numbers::pi / static_cast<double>(numberOfNodes);
    const Mathematics::GeneralizedPolygonTransformation transformation(
        numberOfNodes);

    EXPECT_EQ(expectedLambda, transformation.getLambda());
    EXPECT_EQ(expectedTheta, transformation.getTheta());
  }
}

TEST(GeneralizedPolygonTransformation, getNodesOfTransformedPolygon) {
  // Transform polygons from 3 up to 10 nodes using successively increasing
  // transformation parameters lambda in (0,1) and theta in (0,pi/2).
  const auto nodes = MathematicsTestUtilities::getSampleNodes();
  for (std::size_t numberOfPolygonNodes = 3; numberOfPolygonNodes <= 10;
       ++numberOfPolygonNodes) {
    const auto polygon =
        MathematicsTestUtilities::getPolygon(numberOfPolygonNodes);
    const double lambda = static_cast<double>(numberOfPolygonNodes) * 0.07234;
    const double theta =
        static_cast<double>(numberOfPolygonNodes) * (std::numbers::pi / 22.0);
    const std::vector<Mathematics::Vector2D> polygonNodes(
        nodes.begin(), nodes.begin() + numberOfPolygonNodes);
    const auto expectedTransformedNodes =
        computeTransformedNodesUsingComplexRepresentation(polygonNodes, lambda,
                                                          theta);

    const Mathematics::GeneralizedPolygonTransformation transformation(lambda,
                                                                       theta);
    const auto transformedNodes =
        transformation.getNodesOfTransformedPolygon(polygon, nodes);

    EXPECT_TRUE(Mathematics::areEqual(transformedNodes,
                                      expectedTransformedNodes, 1.0e-13));
  }
}

TEST(GeneralizedPolygonTransformation, getEigenvalues_triangle) {
  const std::size_t numberOfNodes = 3;
  const double lambda = .3;
  const double theta = 0.723 * std::numbers::pi / 2.0;
  const std::vector<double> expectedEigenvalues = {
      1.000000000000000,
      9.782099241153723,
      4.565315750906144,
  };

  const Mathematics::GeneralizedPolygonTransformation transformation(lambda,
                                                                     theta);
  const auto eigenvalues = transformation.getEigenvalues(numberOfNodes);
  const double tolerance = 1.0e-14;
  for (std::size_t k = 0; k < numberOfNodes; ++k) {
    EXPECT_NEAR(expectedEigenvalues.at(k), eigenvalues.at(k), tolerance);
  }
}

TEST(GeneralizedPolygonTransformation, getEigenvalues_14gon) {
  const std::size_t numberOfNodes = 14;
  const double lambda = .77;
  const double theta = 0.334 * std::numbers::pi / 2.0;
  const std::vector<double> expectedEigenvalues = {
      1.000000000000000e+00, 1.083942769210607e+00, 1.088124831730763e+00,
      1.011717878788722e+00, 8.698552444844360e-01, 6.906345633729032e-01,
      5.095526893456727e-01, 3.624751083772805e-01, 2.785323391666739e-01,
      2.743502766465173e-01, 3.507572295885588e-01, 4.926198638928445e-01,
      6.718405450043772e-01, 8.529224190316078e-01,
  };

  const Mathematics::GeneralizedPolygonTransformation transformation(lambda,
                                                                     theta);
  const auto eigenvalues = transformation.getEigenvalues(numberOfNodes);

  const double tolerance = 1.0e-14;
  for (std::size_t k = 0; k < numberOfNodes; ++k) {
    EXPECT_NEAR(expectedEigenvalues.at(k), eigenvalues.at(k), tolerance);
  }
}

TEST(GeneralizedPolygonTransformation,
     isCounterclockwiseRegularizingTransformation) {
  // Use result of Lemma 5.7 of the GETMe book for testing.
  const double lambda = 0.5;
  for (std::size_t numberOfNodes = 3; numberOfNodes <= 10; ++numberOfNodes) {
    const double theta0 =
        1.0 * std::numbers::pi / (2.0 * static_cast<double>(numberOfNodes));
    const double theta1 =
        3.0 * std::numbers::pi / (2.0 * static_cast<double>(numberOfNodes));
    const double thetaIncrement = 0.05;

    double theta = thetaIncrement;
    while (theta < std::numbers::pi / 2.0) {
      const bool isExpectedToBeRegularizing =
          theta >= theta0 && theta <= theta1;
      const Mathematics::GeneralizedPolygonTransformation transformation(lambda,
                                                                         theta);
      const bool isRegularizing =
          transformation.isCounterclockwiseRegularizingTransformation(
              numberOfNodes);
      EXPECT_EQ(isExpectedToBeRegularizing, isRegularizing);
      theta += thetaIncrement;
    }
  }
}
