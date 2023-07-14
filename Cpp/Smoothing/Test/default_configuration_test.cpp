/*
Unit tests for common configuration default.

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

#include "../Source/common_algorithms.h"
#include "Mathematics/generalized_polygon_transformation.h"

#include "gtest/gtest.h"

TEST(DefaultConfiguration,
     getRegularizingPolygonTransformations_genericSetIsRegularizing) {
  using namespace Smoothing::DefaultConfiguration;
  const std::size_t maxNumberOfPolygonNodes = 17;
  const auto set = PolygonTransformationSet::Generic;
  const auto transformations =
      getRegularizingPolygonTransformations(maxNumberOfPolygonNodes, set);
  EXPECT_NO_THROW(Smoothing::checkTransformations(maxNumberOfPolygonNodes,
                                                  transformations));
}

TEST(DefaultConfiguration,
     getRegularizingPolygonTransformations_GETMeBookExamplesSetIsRegularizing) {
  using namespace Smoothing::DefaultConfiguration;
  const std::size_t maxNumberOfPolygonNodes = 17;
  const auto set = PolygonTransformationSet::GETMeBookExamples;
  const auto transformations =
      getRegularizingPolygonTransformations(maxNumberOfPolygonNodes, set);
  EXPECT_NO_THROW(Smoothing::checkTransformations(maxNumberOfPolygonNodes,
                                                  transformations));
}

TEST(DefaultConfiguration,
     getRegularizingPolygonTransformations_throwIfTooShort) {
  using namespace Smoothing::DefaultConfiguration;
  const std::size_t maxNumberOfPolygonNodes = 2;
  const auto set = PolygonTransformationSet::Generic;
  EXPECT_ANY_THROW(
      getRegularizingPolygonTransformations(maxNumberOfPolygonNodes, set));
}
