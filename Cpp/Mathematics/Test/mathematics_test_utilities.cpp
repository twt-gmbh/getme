// Utility functions for mathematics tests.
#include "mathematics_test_utilities.h"

#include "Mathematics/polygon.h"
#include "Mathematics/vector2d.h"
#include "Utility/exception_handling.h"

#include <algorithm>
#include <numeric>

std::vector<Mathematics::Vector2D> MathematicsTestUtilities::getSampleNodes() {
  return std::vector<Mathematics::Vector2D>{
      {6.207745451271750, 6.047292283197333},
      {3.871441731416897, 8.962211743918193},
      {3.851480604892402, 6.612463251649158},
      {6.381051765134135, 2.996953307256387},
      {8.556975503241231, 7.629532136943106},
      {0.938376484716864, 1.081169718593975},
      {3.975393812489129, 6.204528406802986},
      {6.309683676346705, 9.709797463627504},
      {6.725611567202442, 0.541767102371060},
      {8.335806041391313, 8.285845003094725},
  };
}

Mathematics::Polygon MathematicsTestUtilities::getPolygon(
    const std::size_t numberOfNodes) {
  Utility::throwExceptionIfFalse(numberOfNodes >= 3 && numberOfNodes <= 10,
                                 "number of nodes must be in [3,10]");
  std::vector<std::size_t> nodeIndices(numberOfNodes, 0);
  std::iota(nodeIndices.begin(), nodeIndices.end(), 0);
  return Mathematics::Polygon(nodeIndices);
}

std::vector<Mathematics::Vector2D>
MathematicsTestUtilities::complexToVector2DNodes(
    const std::vector<std::complex<double>>& complexNodes) {
  std::vector<Mathematics::Vector2D> vector2DNodes;
  vector2DNodes.reserve(complexNodes.size());
  std::ranges::transform(complexNodes, std::back_inserter(vector2DNodes),
                         [](const std::complex<double>& complexNode) {
                           return Mathematics::Vector2D(complexNode.real(),
                                                        complexNode.imag());
                         });
  return vector2DNodes;
}

std::vector<std::complex<double>>
MathematicsTestUtilities::vector2DToComplexNodes(
    const std::vector<Mathematics::Vector2D>& vector2DNodes) {
  std::vector<std::complex<double>> complexNodes;
  complexNodes.reserve(vector2DNodes.size());
  std::ranges::transform(vector2DNodes, std::back_inserter(complexNodes),
                         [](const Mathematics::Vector2D& vector2DNode) {
                           return std::complex<double>(vector2DNode.getX(),
                                                       vector2DNode.getY());
                         });
  return complexNodes;
}
