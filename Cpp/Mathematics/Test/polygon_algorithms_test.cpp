// Unit tests for polygonal element algorithms.
#include "Mathematics/polygon_algorithms.h"

#include "Mathematics/polygon.h"
#include "Mathematics/vector2d_algorithms.h"
#include "Utility/generic_exception.h"
#include "mathematics_test_utilities.h"

#include "gtest/gtest.h"

TEST(PolygonAlgorithms, getNodesOfRegularPolygon) {
  EXPECT_THROW(Mathematics::getNodesOfRegularPolygon(2),
               Utility::GenericException);

  std::vector<std::vector<Mathematics::Vector2D>> expectedRegularPolygonNodes{
      {
          {+1.0, +0.0},
          {-0.5, +8.660254037844387e-01},
          {-0.5, -8.660254037844384e-01},
      },
      {
          {+1.0, +0.0},
          {+0.0, +1.0},
          {-1.0, +0.0},
          {+0.0, -1.0},
      },
      {
          {+1.0, +0.0},
          {+6.234898018587336e-01, +7.818314824680298e-01},
          {-2.225209339563143e-01, +9.749279121818237e-01},
          {-9.009688679024191e-01, +4.338837391175583e-01},
          {-9.009688679024195e-01, -4.338837391175580e-01},
          {-2.225209339563148e-01, -9.749279121818237e-01},
          {+6.234898018587334e-01, -7.818314824680304e-01},
      },
  };

  for (const auto& expectedNodes : expectedRegularPolygonNodes) {
    const auto nodes =
        Mathematics::getNodesOfRegularPolygon(expectedNodes.size());
    const double tolerance = 1.0e-15;
    EXPECT_TRUE(Mathematics::areEqual(expectedNodes, nodes, tolerance));
  }
}

TEST(PolygonAlgorithms, getMeanRatio) {
  const auto nodes = MathematicsTestUtilities::getSampleNodes();
  std::vector<std::pair<std::vector<std::size_t>, double>>
      polygonNodeIndicesAndExpectedMeanRatioValues = {
          {{8, 2, 3}, -1.0},
          {{6, 5, 8}, 9.954063108377308e-01},
          {{4, 0, 7, 1}, -1.0},
          {{3, 9, 7, 5}, 6.193918675611152e-01},
          {{7, 2, 4, 9, 3}, -1.0},
          {{9, 1, 4, 7, 8}, 3.485354135089186e-01},
          {{2, 0, 5, 9, 6, 3}, -1.0},
          {{2, 3, 1, 5, 4, 7}, 5.103058748841401e-01},
          {{9, 2, 3, 0, 4, 6, 8}, -1.0},
          {{8, 7, 2, 0, 4, 1, 6}, 4.691307597322946e-01},
      };

  for (const auto& [polygonNodeIndices, expectedMeanRatioValue] :
       polygonNodeIndicesAndExpectedMeanRatioValues) {
    const Mathematics::Polygon polygon(polygonNodeIndices);
    const double meanRatioValue = Mathematics::getMeanRatio(polygon, nodes);
    const double tolerance = 1.0e-15;

    EXPECT_NEAR(expectedMeanRatioValue, meanRatioValue, tolerance);
  }
}
