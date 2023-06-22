// Configuration of the GETMe sequential algorithm.
// Cf. Section 6.1.3 of the GETMe book.
#pragma once

#include "Mathematics/generalized_polygon_transformation.h"
#include "Smoothing/default_configuration.h"

#include <vector>

namespace Smoothing {

struct GetmeSequentialConfig final {
  explicit GetmeSequentialConfig(
      const std::size_t maxNumberOfPolygonNodes,
      const DefaultConfiguration::PolygonTransformationSet transformationSet =
          DefaultConfiguration::PolygonTransformationSet::GETMeBookExamples)
    : polygonTransformations(
        DefaultConfiguration::getRegularizingPolygonTransformations(
            maxNumberOfPolygonNodes,
            transformationSet)) {}

  // Since the lowest quality polygons are addressed, use conservative
  // relaxation parameter to avoid neighbor element invalidation.
  double relaxationParameterRho = 0.01;

  // Terminate if the number of iterations exceeds this limit.
  // Since only one polygon is transformed by one iteration, this limit is
  // usually much larger than that of GETMe simultaneous.
  std::size_t maxIterations = 1'000'000;

  // Evaluate mesh quality after the given number of smoothing iterations, which
  // define one smoothing cycle.
  std::size_t qualityEvaluationCycleLength = 100;

  // Terminate if the given maximal number of smoothing cycles did not lead to
  // q_min* improvements.
  std::size_t maxNoImprovementCycles = 20;

  // Polygon quality penalty values to be applied depending on polygon selection
  // and transformed nodes applications success.
  double penaltyInvalid = 1.0e-4;
  double penaltyRepeated = 1.0e-5;
  double penaltySuccess = 1.0e-3;

  // Regularizing transformations to apply.
  std::vector<Mathematics::GeneralizedPolygonTransformation>
      polygonTransformations;
};
}  // namespace Smoothing
