// Generalized planar polygon transformations according to Definition 5.4 of the
// GETMe book.
#include "Mathematics/generalized_polygon_transformation.h"

#include "Mathematics/polygon.h"
#include "Mathematics/vector2d.h"
#include "Utility/exception_handling.h"

#include <algorithm>
#include <cmath>
#include <complex>
#include <numbers>

namespace Mathematics {
GeneralizedPolygonTransformation::GeneralizedPolygonTransformation(
    const double lambda,
    const double theta)
  : lambda(lambda)
  , theta(theta)
  , c1((1.0 - lambda) * std::tan(theta))
  , c2(lambda * (1.0 - lambda) - c1 * c1)
  , c3(1.0 - 2.0 * c2) {
  Utility::throwExceptionIfFalse(lambda > 0.0 && lambda < 1.0,
                                 "Lambda has to be in (0,1).");
  Utility::throwExceptionIfFalse(theta > 0.0 && theta < std::numbers::pi / 2.0,
                                 "Theta has to be in (0,pi/2).");
}

GeneralizedPolygonTransformation::GeneralizedPolygonTransformation(
    const std::size_t numberOfPolygonNodes)
  : GeneralizedPolygonTransformation(
      0.5,
      numberOfPolygonNodes < 3
          ? std::numbers::pi / 4.0
          : std::numbers::pi / static_cast<double>(numberOfPolygonNodes)){};

std::vector<Vector2D>
GeneralizedPolygonTransformation::getNodesOfTransformedPolygon(
    const Polygon& polygon,
    const std::vector<Vector2D>& nodes) const {
  std::vector<Vector2D> transformedNodes(polygon.getNumberOfNodes(),
                                         Vector2D(0.0, 0.0));
  for (std::size_t nodeNumber = 0; nodeNumber < polygon.getNumberOfNodes();
       ++nodeNumber) {
    const auto& predecessorNode =
        nodes.at(polygon.getPredecessorNodeIndex(nodeNumber));
    const auto& node = nodes.at(polygon.getNodeIndex(nodeNumber));
    const auto& successorNode =
        nodes.at(polygon.getSuccessorNodeIndex(nodeNumber));
    // Compute new node position according to Equation 5.26 of the GETMe book.
    transformedNodes.at(nodeNumber) =
        c1
            * Vector2D(successorNode.getY() - predecessorNode.getY(),
                       predecessorNode.getX() - successorNode.getX())
        + c2 * (predecessorNode + successorNode) + c3 * node;
  }
  return transformedNodes;
}

std::vector<double> GeneralizedPolygonTransformation::getEigenvalues(
    const std::size_t numberOfPolygonNodes) const {
  using namespace std::complex_literals;
  std::vector<double> eigenvalues(numberOfPolygonNodes, 0.0);
  // Transformation parameter according to Definition 5.1 and
  // complex root of unity.
  const auto w = std::complex<double>(lambda, (1 - lambda) * std::tan(theta));
  const auto r = std::exp(2.0i * std::numbers::pi
                          / static_cast<double>(numberOfPolygonNodes));
  // Eigenvalue computation according to Equation 5.11 of the GETMe book.
  for (std::size_t k = 0; k < numberOfPolygonNodes; ++k) {
    const auto magnitude =
        std::abs(1.0 - std::conj(w)
                 + std::pow(r, static_cast<double>(k)) * std::conj(w));
    eigenvalues.at(k) = magnitude * magnitude;
  }
  return eigenvalues;
}

bool GeneralizedPolygonTransformation::
    isCounterclockwiseRegularizingTransformation(
        const std::size_t numberOfPolygonNodes) const {
  const auto eigenvalues = getEigenvalues(numberOfPolygonNodes);
  const double expectedDominantEigenvalue = eigenvalues.at(1);
  const auto isLessOrEqualToDominating =
      [expectedDominantEigenvalue](const double eigenvalue) {
        return expectedDominantEigenvalue >= eigenvalue;
      };
  return std::ranges::all_of(eigenvalues, isLessOrEqualToDominating);
}
}  // namespace Mathematics
