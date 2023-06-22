// Algorithms for planar polygons.
#include "Mathematics/polygon_algorithms.h"

#include "Mathematics/polygon.h"
#include "Mathematics/vector2d.h"
#include "Utility/exception_handling.h"

#include <cmath>
#include <numbers>

std::vector<Mathematics::Vector2D> Mathematics::getNodesOfRegularPolygon(
    const std::size_t numberOfNodes) {
  Utility::throwExceptionIfTrue(numberOfNodes < 3,
                                "at least three nodes required");
  std::vector<Vector2D> nodes(numberOfNodes, Mathematics::Vector2D(0.0, 0.0));
  for (std::size_t index = 0; index < numberOfNodes; ++index) {
    const double angle = static_cast<double>(index) * 2.0 * std::numbers::pi
                         / static_cast<double>(numberOfNodes);
    nodes.at(index) = Mathematics::Vector2D(std::cos(angle), std::sin(angle));
  }
  return nodes;
}

namespace {
// Compute one summand of Equation (2.6) of the GETMe book:
// q(E):=(2/numberOfPolygonNodes) sum_{0}^{numberOfPolygonNodes-1}
// det(S_k)/trace(S_k^tS_k) with S_k:=D(T_k)W^{-1}. Here, zero based node
// indices are used and m=2 is considered in the factor in front of the sum.
double getMeanRatioSummand(const Mathematics::Polygon& polygon,
                           const std::size_t polygonNodeNumber,
                           const std::vector<Mathematics::Vector2D>& nodes,
                           const std::size_t numberOfPolygonNodes) {
  const std::size_t predecessorNodeIndex =
      polygon.getPredecessorNodeIndex(polygonNodeNumber);
  const std::size_t centerNodeIndex = polygon.getNodeIndex(polygonNodeNumber);
  const std::size_t successorNodeIndex =
      polygon.getSuccessorNodeIndex(polygonNodeNumber);

  // Nodes of the reference triangle for a regular n-gon with
  // centroid (0,0) and radius 1:
  // predecessor node:  [cos(2*pi/n); -sin(2*pi/n)]
  // center node     :  [1;0]
  // successor node  :  [cos(2*pi/n); sin(2*pi/n)]
  // Matrix W: [successor-center, predecessor-center] =
  // [cos(2*pi/n)-1,cos(2*pi/n)-1;sin(2*pi/n),-sin(2*pi/n)] =: [a,a;b,-b]
  const double regularPolygonAngle =
      2.0 * std::numbers::pi / static_cast<double>(numberOfPolygonNodes);
  const double a = std::cos(regularPolygonAngle) - 1.0;
  const double b = std::sin(regularPolygonAngle);
  const auto diffSuccessorCenter =
      nodes.at(successorNodeIndex) - nodes.at(centerNodeIndex);
  const auto diffPredecessorCenter =
      nodes.at(predecessorNodeIndex) - nodes.at(centerNodeIndex);

  // Entries of the matrix D(T_k) := [successor-center, predecessor-center] are
  // [d11,d12; d21,d22].
  const double d11 = diffSuccessorCenter.getX();
  const double d12 = diffPredecessorCenter.getX();
  const double d21 = diffSuccessorCenter.getY();
  const double d22 = diffPredecessorCenter.getY();

  // Compute the determinant of S_k := D(T_k)*W^(-1), which is
  // det(S_k) = det(D(T_k))*(1/det(W)).
  const double detS = (d12 * d21 - d11 * d22) / (2.0 * a * b);
  if (detS < 0.0) {
    return -1.0;
  }
  const double trace =
      ((d11 - d12) * (d11 - d12) + (d21 - d22) * (d21 - d22)) / (4.0 * b * b)
      + ((d11 + d12) * (d11 + d12) + (d21 + d22) * (d21 + d22)) / (4.0 * a * a);
  const double summand = detS / trace;
  return summand;
}
}  // namespace

double Mathematics::getMeanRatio(const Polygon& polygon,
                                 const std::vector<Vector2D>& nodes) {
  // Note: 1.0 will be enforced as exact upper bound to cut off numerical
  // inaccuracies.
  const auto numberOfNodes = polygon.getNumberOfNodes();
  if (numberOfNodes == 3) {
    // Special case triangle: all node simplices are the same. Therefore it
    // suffices to compute only one summand. Hence, no division by three.
    const double summand =
        getMeanRatioSummand(polygon, 0, nodes, numberOfNodes);
    return summand < 0.0 ? -1.0 : std::min(1.0, 2.0 * summand);
  } else {
    double sum = 0.0;
    for (std::size_t nodeNumber = 0; nodeNumber < numberOfNodes; ++nodeNumber) {
      const double summand =
          getMeanRatioSummand(polygon, nodeNumber, nodes, numberOfNodes);
      if (summand < 0.0) {
        return -1.0;
      }
      sum += summand;
    }
    return std::min(1.0, 2.0 * sum / static_cast<double>(numberOfNodes));
  }
}
