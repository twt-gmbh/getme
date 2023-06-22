// Configuration of the basic GETMe simultaneous smoothing algorithm not
// involving element quality. Cf. Section 6.1.1 of the GETMe book.
#pragma once

#include "Mathematics/generalized_polygon_transformation.h"
#include "Smoothing/default_configuration.h"

#include <vector>

namespace Smoothing {
struct BasicGetmeSimultaneousConfig final {
  explicit BasicGetmeSimultaneousConfig(
      const double maxNodeRelocationDistanceThreshold,
      const std::size_t maxNumberOfPolygonNodes,
      const DefaultConfiguration::PolygonTransformationSet transformationSet =
          DefaultConfiguration::PolygonTransformationSet::GETMeBookExamples)
    : maxSquaredNodeRelocationDistanceThreshold(
        maxNodeRelocationDistanceThreshold * maxNodeRelocationDistanceThreshold)
    , polygonTransformations(
          DefaultConfiguration::getRegularizingPolygonTransformations(
              maxNumberOfPolygonNodes,
              transformationSet)) {}

  // Terminated if the maximal distance of a node relocation drops below the
  // given threshold. This threshold depends on the scale of the mesh elements.
  // It is squared to avoid square root evaluations.
  double maxSquaredNodeRelocationDistanceThreshold;

  // Terminate if the number of iterations exceeds this limit.
  std::size_t maxIterations = DefaultConfiguration::maxIterations;

  // Regularizing transformations to apply.
  std::vector<Mathematics::GeneralizedPolygonTransformation>
      polygonTransformations;
};
}  // namespace Smoothing
