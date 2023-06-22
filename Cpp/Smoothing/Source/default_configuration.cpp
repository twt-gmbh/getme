// Common defaults for smoothing algorithm configurations.
#include "Smoothing/default_configuration.h"

#include "Utility/exception_handling.h"

#include <numbers>

namespace Smoothing {
std::vector<Mathematics::GeneralizedPolygonTransformation>
DefaultConfiguration::getRegularizingPolygonTransformations(
    const std::size_t maxNumberOfPolygonNodes,
    const PolygonTransformationSet transformationSet) {
  Utility::throwExceptionIfFalse(
      maxNumberOfPolygonNodes >= 3,
      "Minimal valid number of polygon nodes is three.");
  std::vector<Mathematics::GeneralizedPolygonTransformation>
      polygonTransformations;
  polygonTransformations.reserve(maxNumberOfPolygonNodes + 1);
  for (std::size_t numberOfPolygonNodes = 0;
       numberOfPolygonNodes <= maxNumberOfPolygonNodes;
       ++numberOfPolygonNodes) {
    polygonTransformations.emplace_back(numberOfPolygonNodes);
  }
  if (transformationSet == PolygonTransformationSet::GETMeBookExamples) {
    const double lambda = 0.5;
    polygonTransformations.at(3) =
        Mathematics::GeneralizedPolygonTransformation(lambda,
                                                      std::numbers::pi / 4.0);
    if (maxNumberOfPolygonNodes >= 4) {
      polygonTransformations.at(4) =
          Mathematics::GeneralizedPolygonTransformation(lambda,
                                                        std::numbers::pi / 6.0);
    }
  }
  return polygonTransformations;
}
}  // namespace Smoothing
