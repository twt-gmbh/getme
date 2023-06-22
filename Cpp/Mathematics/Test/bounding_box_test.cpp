// Unit tests for bounding boxes.
#include "Mathematics/bounding_box.h"

#include "gtest/gtest.h"

#include <vector>

TEST(BoundingBox, basicGetters) {
  const double xMin = 4.0;
  const double xMax = 8.0;
  const double yMin = -2.0;
  const double yMax = 6.0;
  const Mathematics::BoundingBox boundingBox(xMin, xMax, yMin, yMax);

  EXPECT_EQ(xMin, boundingBox.getXMin());
  EXPECT_EQ(xMax, boundingBox.getXMax());
  EXPECT_EQ(yMin, boundingBox.getYMin());
  EXPECT_EQ(yMax, boundingBox.getYMax());

  EXPECT_EQ(xMax - xMin, boundingBox.getXDimension());
  EXPECT_EQ(yMax - yMin, boundingBox.getYDimension());
}

TEST(BoundingBox, validityChecks) {
  EXPECT_ANY_THROW(Mathematics::BoundingBox(2.0, -1.0, 0.0, 1.0));
  EXPECT_ANY_THROW(Mathematics::BoundingBox(2.0, 4.0, 4.0, 1.0));
  EXPECT_ANY_THROW(Mathematics::BoundingBox(8.0, 4.0, 4.0, 1.0));
  EXPECT_NO_THROW(Mathematics::BoundingBox(2.0, 4.0, 0.0, 1.0));
  EXPECT_NO_THROW(Mathematics::BoundingBox(1.0, 1.0, 2.0, 2.0));
}

TEST(BoundingBox, equalsOperator) {
  EXPECT_TRUE((Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.0}
               == Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.0}));
  EXPECT_FALSE((Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.0}
                == Mathematics::BoundingBox{1.1, 2.0, 3.0, 4.0}));
  EXPECT_FALSE((Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.0}
                == Mathematics::BoundingBox{1.0, 2.2, 3.0, 4.0}));
  EXPECT_FALSE((Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.0}
                == Mathematics::BoundingBox{1.0, 2.0, 3.3, 4.0}));
  EXPECT_FALSE((Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.0}
                == Mathematics::BoundingBox{1.0, 2.0, 3.0, 4.4}));
}
