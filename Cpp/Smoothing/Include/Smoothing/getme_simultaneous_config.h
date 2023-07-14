/*
Configuration of the GETMe simultaneous smoothing algorithm

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
// using element quality. Cf. Section 6.1.2 of the GETMe book.
#pragma once

#include "Mathematics/generalized_polygon_transformation.h"
#include "Smoothing/default_configuration.h"

#include <vector>

namespace Smoothing {
struct GetmeSimultaneousConfig final {
  explicit GetmeSimultaneousConfig(
      const std::size_t maxNumberOfPolygonNodes,
      const DefaultConfiguration::PolygonTransformationSet transformationSet =
          DefaultConfiguration::PolygonTransformationSet::GETMeBookExamples)
    : polygonTransformations(
        DefaultConfiguration::getRegularizingPolygonTransformations(
            maxNumberOfPolygonNodes,
            transformationSet)) {}

  // Use default parameters for eta and rho, which simplify the algorithm
  // but usually result in slightly lower mesh qualities.
  double weightExponentEta = 0.0;
  double relaxationParameterRho = 1.0;

  // Terminate if the improvement of the arithmetic mean of all element quality
  // numbers of one iteration drops below the given threshold.
  double qMeanImprovementThreshold =
      DefaultConfiguration::qMeanImprovementThreshold;

  // Terminate if the number of iterations exceeds this limit.
  std::size_t maxIterations = DefaultConfiguration::maxIterations;

  // Regularizing transformations to apply.
  std::vector<Mathematics::GeneralizedPolygonTransformation>
      polygonTransformations;
};
}  // namespace Smoothing
