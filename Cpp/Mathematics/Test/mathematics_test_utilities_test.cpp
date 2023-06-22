// Unit tests for mathematics utility functions.
#include "mathematics_test_utilities.h"

#include "Mathematics/polygon.h"
#include "Mathematics/vector2d.h"

#include "gtest/gtest.h"

#include <vector>

namespace {
const std::vector<std::complex<double>> complexNodes{{0.0, 0.0},
                                                     {1.0, -2.0},
                                                     {-2.0, 1.0},
                                                     {-3.0, -4.0}};
const std::vector<Mathematics::Vector2D> vector2DNodes{{0.0, 0.0},
                                                       {1.0, -2.0},
                                                       {-2.0, 1.0},
                                                       {-3.0, -4.0}};
}  // namespace

TEST(MathematicsTestUtilities, getPolygon) {
  EXPECT_EQ(Mathematics::Polygon({0, 1, 2}),
            MathematicsTestUtilities::getPolygon(3));
  EXPECT_EQ(Mathematics::Polygon({0, 1, 2, 3, 4, 5}),
            MathematicsTestUtilities::getPolygon(6));
}

TEST(MathematicsTestUtilities, complexToVector2DNodes) {
  EXPECT_EQ(vector2DNodes,
            MathematicsTestUtilities::complexToVector2DNodes(complexNodes));
}

TEST(MathematicsTestUtilities, vector2DToComplexNodes) {
  EXPECT_EQ(complexNodes,
            MathematicsTestUtilities::vector2DToComplexNodes(vector2DNodes));
}
