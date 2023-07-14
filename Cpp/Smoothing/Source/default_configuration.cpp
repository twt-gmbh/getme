/*
Common defaults for smoothing algorithm configurations.

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
