/*
Configuration of the GETMe sequential algorithm.

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
