// Unit tests for common configuration default.
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
